// File: runner.hpp
// Project: examples
// Created Date: 16/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 29/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>

#include "autd3.hpp"
#include "autd3/liblsl/lsl_cpp.h"
#include "tests/advanced.hpp"
#include "tests/bessel.hpp"
#include "tests/flag.hpp"
#include "tests/focus.hpp"
#include "tests/gain_stm.hpp"
#include "tests/group.hpp"
#include "tests/plane.hpp"
#include "tests/soft_stm.hpp"
#ifdef BUILD_GAIN_HOLO
#include "tests/holo.hpp"
#endif
#ifdef BUILD_MODULATION_AUDIO_FILE
#endif
#include "tests/focus_stm.hpp"
#include "tests/am_single.hpp"
#include "tests/dummy.hpp"
#include "tests/lmc.hpp"

#include <chrono>
#include <thread>
#include <random>
#include <atomic>
#include <thread>
#include <mutex>

using std::this_thread::sleep_for;

std::vector<int> buttonPressedTrials;
std::mutex mtx;
std::atomic<int> currentTrial(-1);
std::atomic<bool> isRunning(true);

void monitorSerialPort(HANDLE hComm)
{
    while (isRunning)
    {
        DWORD errors;
        COMSTAT status;
        ClearCommError(hComm, &errors, &status);
        if (status.cbInQue > 0)
        {
            char TempChar;
            DWORD NoBytesRead;
            if (ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL) && NoBytesRead > 0)
            {
                if (TempChar == '1')
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    if (currentTrial != -1)
                    {
                        buttonPressedTrials.push_back(currentTrial);
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Reduce CPU usage
    }
}

inline int run(autd3::Controller &autd)
{
    using F = std::function<void(autd3::Controller &)>;

    autd.set_sound_speed(340.0e3);

    const auto firm_infos = autd.firmware_infos();
    if (firm_infos.empty())
        throw std::runtime_error("Cannot read firmware information.");
    std::cout << "================================== AUTD3 firmware information ==================================" << std::endl;
    std::copy(firm_infos.begin(), firm_infos.end(), std::ostream_iterator<autd3::FirmwareInfo>(std::cout, "\n"));
    std::cout << "================================================================================================" << std::endl;

    // BrainAmp用のシリアルポートを開く
    HANDLE hComm3;
    hComm3 = CreateFile("COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hComm3 == INVALID_HANDLE_VALUE)
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

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hComm3, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hComm3, &dcbSerialParams);

    // Arduino Button用のシリアルポートを開く
    HANDLE hComm4;
    hComm4 = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hComm4 == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            printf("Serial port does not exist.\n");
        }
        printf("Failed to open serial port.\n");
    }
    else
    {
        printf("COM4 port found.\n");
    }

    DCB dcbSerialParams4 = {0};
    dcbSerialParams4.DCBlength = sizeof(dcbSerialParams4);
    GetCommState(hComm4, &dcbSerialParams4);
    dcbSerialParams4.BaudRate = CBR_115200;
    dcbSerialParams4.ByteSize = 8;
    dcbSerialParams4.StopBits = ONESTOPBIT;
    dcbSerialParams4.Parity = NOPARITY;
    SetCommState(hComm4, &dcbSerialParams4);

    // BrainAmp用トリガーの用意
    std::string dataToSendRR = "RR";
    std::string dataToSend0 = "00";
    std::string dataToSend1 = "01";
    std::string dataToSend2 = "02";
    std::string dataToSend3 = "03";
    std::string dataToSend4 = "04";
    std::string dataToSend5 = "05";
    std::string dataToSend6 = "06";
    std::string dataToSend7 = "07";
    std::string dataToSend8 = "08";
    DWORD bytesWritten;

    // NOTE: BrainAmpではRR2回送る
    WriteFile(hComm, dataToSendRR.c_str(), dataToSendRR.size(), &bytesWritten, NULL);
    WriteFile(hComm, dataToSendRR.c_str(), dataToSendRR.size(), &bytesWritten, NULL);

    autd << autd3::clear << autd3::synchronize;

    std::mt19937_64 mt64(0);
    std::uniform_real_distribution<double> uni(0, 1);

    std::array<int, 350> trials;
    std::fill(trials.begin(), trials.begin() + 50, 0);
    std::fill(trials.begin() + 50, trials.begin() + 100, 1);
    std::fill(trials.begin() + 100, trials.begin() + 150, 2);
    std::fill(trials.begin() + 150, trials.begin() + 200, 3);
    std::fill(trials.begin() + 200, trials.begin() + 250, 4);
    std::fill(trials.begin() + 250, trials.begin() + 300, 5);
    std::fill(trials.begin() + 300, trials.end(), 6);
    std::shuffle(trials.begin(), trials.end(), mt64);

    std::thread serialThread(monitorSerialPort, hComm); // 別スレッドでArduino Button用シリアルポートを監視

    for (int trial : trials)
    {
        currentTrial = trial;

        if (trial == 0)
        {
            WriteFile(hComm, dataToSend1.c_str(), dataToSend1.size(), &bytesWritten, NULL);
            F{am_single_50}(autd);
            WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
        }
        else if (trial == 1)
        {
            WriteFile(hComm, dataToSend2.c_str(), dataToSend2.size(), &bytesWritten, NULL);
            F{am_single_150}(autd);
            WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
        }
        else if (trial == 2)
        {
            WriteFile(hComm, dataToSend3.c_str(), dataToSend3.size(), &bytesWritten, NULL);
            F{lm_circle_5_5}(autd);
            WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
        }
        else if (trial == 3)
        {
            WriteFile(hComm, dataToSend4.c_str(), dataToSend4.size(), &bytesWritten, NULL);
            F{lm_circle_15_5}(autd);
            WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
        }
        else if (trial == 4)
        {
            WriteFile(hComm, dataToSend5.c_str(), dataToSend5.size(), &bytesWritten, NULL);
            F{lm_circle_5_30}(autd);
            WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
        }
        else if (trial == 5)
        {
            WriteFile(hComm, dataToSend6.c_str(), dataToSend6.size(), &bytesWritten, NULL);
            F{lm_circle_15_30}(autd);
            WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
        }
        else if (trial == 6)
        {
            WriteFile(hComm, dataToSend7.c_str(), dataToSend7.size(), &bytesWritten, NULL);
            F{dummy}(autd);
            WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
        }

        sleep_for(std::chrono::milliseconds(1000));
        WriteFile(hComm, dataToSend8.c_str(), dataToSend8.size(), &bytesWritten, NULL);
        sleep_for(std::chrono::milliseconds(10));
        WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);

        sleep_for(std::chrono::milliseconds(1500 + int(uni(mt64) * 1000)));
    }

    isRunning = false;
    serialThread.join();

    autd.close();
    CloseHandle(hComm3);
    CloseHandle(hComm4);

    for (int pressedTrial : buttonPressedTrials)
    {
        std::cout << "Button pressed during trial: " << pressedTrial << std::endl;
    }

    return 0;
}
