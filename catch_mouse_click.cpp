#include <windows.h>
#include <chrono>
using std::this_thread::sleep_for;

HWND hwndMessageWindow;
HANDLE hComm;
LRESULT CALLBACK MouseWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::string dataToSend0 = "00";
    std::string dataToSend9 = "09";
    switch (msg)
    {
    case WM_INPUT:
    {
        UINT dwSize = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
        if (dwSize > 0)
        {
            std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dwSize);
            if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawdata.get(), &dwSize, sizeof(RAWINPUTHEADER)) == dwSize)
            {
                RAWINPUT *raw = reinterpret_cast<RAWINPUT *>(rawdata.get());
                if (raw->header.dwType == RIM_TYPEMOUSE && raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
                {
                    WriteFile(hComm, dataToSend9.c_str(), dataToSend0.size(), &bytesWritten, NULL);
                    sleep_for(std::chrono::milliseconds(10));
                    WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
                    std::cout << "Left mouse button down (Raw Input)." << std::endl;
                }
            }
        }
        break;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void MessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// マウスのRaw Inputを登録する関数
void RegisterRawInputDevices(HWND hwnd)
{
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = 0x01;        // Generic Desktop Controls
    Rid[0].usUsage = 0x02;            // Mouse
    Rid[0].dwFlags = RIDEV_INPUTSINK; // このウィンドウがフォーカスを持っていなくても入力を受け取る
    Rid[0].hwndTarget = hwnd;
    if (!RegisterRawInputDevices(Rid, 1, sizeof(RAWINPUTDEVICE)))
    {
        std::cerr << "Failed to register raw input device." << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main()
{
    // シリアルポートを開く
    HANDLE hComm;
    hComm = CreateFile("COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hComm == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            printf("Serial port does not exist.\n");
        }
        printf("Failed to open serial port.\n");
    }
    else
    {
        printf("COM3 port found.\n");
    }

    // COMポートの設定
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hComm, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hComm, &dcbSerialParams);

    // トリガーの用意
    std::string dataToSendRR = "RR";
    DWORD bytesWritten;

    // BrainAmpではRR2回送る
    WriteFile(hComm, dataToSendRR.c_str(), dataToSendRR.size(), &bytesWritten, NULL);
    WriteFile(hComm, dataToSendRR.c_str(), dataToSendRR.size(), &bytesWritten, NULL);

    WNDCLASS wndClass = {0};
    wndClass.lpfnWndProc = MouseWindowProcedure;
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpszClassName = "MouseMessageWindowClass";

    if (!RegisterClass(&wndClass))
    {
        return -1;
    }

    hwndMessageWindow = CreateWindowEx(
        0,
        "MouseMessageWindowClass",
        "Hidden message window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwndMessageWindow)
    {
        return -1;
    }

    RegisterRawInputDevices(hwndMessageWindow);

    MessageLoop();
}