#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>

#include <chrono>
#include <thread>
#include <random>
#include <fstream>
#include <ctime>

using std::this_thread::sleep_for;

// cl /EHsc catch_mouse_click.cpp /link user32.lib

HWND hwndMessageWindow;
HANDLE hComm;
std::ofstream file;

void write_timestamp(std::ofstream& file) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    char buffer[32];
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm)) {
        file << buffer << std::endl;  // CSVに書き込む形式を指定
    }
}

LRESULT CALLBACK MouseWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::string dataToSend0 = "00";
    std::string dataToSend9 = "09";
    DWORD bytesWritten;
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
                if (raw->header.dwType == RIM_TYPEMOUSE && raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
                {
                    write_timestamp(file);
                    std::cout << "Right mouse button down (Raw Input)." << std::endl;
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
    file.open("C:/Users/user/Desktop/master_thesis_data/1116/timestamp6.csv", std::ios::out | std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open the CSV file." << std::endl;
        return -1;
    }

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
