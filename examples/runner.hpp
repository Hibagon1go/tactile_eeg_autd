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

#include <chrono>
#include <thread>
#include<iostream>
#include <random>

using std::this_thread::sleep_for;

inline int run(autd3::Controller& autd) {
  using F = std::function<void(autd3::Controller&)>;

  autd.set_sound_speed(340.0e3);

  const auto firm_infos = autd.firmware_infos();
  if (firm_infos.empty()) throw std::runtime_error("Cannot read firmware information.");
  std::cout << "================================== AUTD3 firmware information ==================================" << std::endl;
  std::copy(firm_infos.begin(), firm_infos.end(), std::ostream_iterator<autd3::FirmwareInfo>(std::cout, "\n"));
  std::cout << "================================================================================================" << std::endl;

  // トリガー準備

  
  // シリアルポートを開く
  HANDLE hComm;
  hComm = CreateFile("COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if (hComm == INVALID_HANDLE_VALUE) {
      if (GetLastError() == ERROR_FILE_NOT_FOUND) {
          // シリアルポートが存在しない場合のエラーメッセージ
          printf("Serial port does not exist.\n");
      }
      // 他のエラー
      printf("Failed to open serial port.\n");
  }
  else {
      printf("COM3 port found.\n");
  }

  // COMポートの設定
  DCB dcbSerialParams = { 0 };
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  GetCommState(hComm, &dcbSerialParams);
  dcbSerialParams.BaudRate = CBR_115200;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  SetCommState(hComm, &dcbSerialParams);


  // トリガーの用意
  std::string dataToSendRR = "RR";
  std::string dataToSend0 = "00";
  std::string dataToSend1 = "01";
  std::string dataToSend2 = "02";
  std::string dataToSend3 = "03";
  std::string dataToSend4 = "04";
  DWORD bytesWritten;

  // BrainAmpではRR2回送る
  
  WriteFile(hComm, dataToSendRR.c_str(), dataToSendRR.size(), &bytesWritten, NULL);
  WriteFile(hComm, dataToSendRR.c_str(), dataToSendRR.size(), &bytesWritten, NULL);
  

  /*
  lsl::stream_info info("LSLMarkersInletStreamName1", "Markers", 32, 100, lsl::cf_float32, "NIC");
  lsl::stream_outlet outlet(info, 0, 360);
  std::vector<float> sample1(32, 1.0);
  std::vector<float> sample2(32, 2.0);
  std::vector<float> sample3(32, 3.0);
  std::vector<float> sample4(32, 4.0);
  */
  

  autd << autd3::clear << autd3::synchronize;

  std::mt19937_64 mt64(0);
  std::uniform_real_distribution<double> uni(0, 1);

  std::array<int, 150> trials;
  std::fill(trials.begin(), trials.begin() + 50, 0);
  std::fill(trials.begin() + 50, trials.begin() + 100, 1);
  std::fill(trials.begin() + 100, trials.end(), 2);

  std::shuffle(trials.begin(), trials.end(), mt64);

  for (int trial : trials) {

      if (trial == 0) {
          WriteFile(hComm, dataToSend1.c_str(), dataToSend1.size(), &bytesWritten, NULL);
          //outlet.push_sample(sample1);
          F{ focus_test_weak }(autd);
      }
      else if (trial == 1) {
          WriteFile(hComm, dataToSend2.c_str(), dataToSend2.size(), &bytesWritten, NULL);
          //outlet.push_sample(sample2);
          F{ focus_test_strong }(autd);
      }
      else if (trial == 2) {
          WriteFile(hComm, dataToSend3.c_str(), dataToSend3.size(), &bytesWritten, NULL);
          //outlet.push_sample(sample3);
          F{ focus_stm }(autd);
      }

      WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
      sleep_for(std::chrono::milliseconds(500));

      autd << autd3::stop;

      sleep_for(std::chrono::milliseconds(1000));
      WriteFile(hComm, dataToSend4.c_str(), dataToSend0.size(), &bytesWritten, NULL);
      sleep_for(std::chrono::milliseconds(10));
      WriteFile(hComm, dataToSend0.c_str(), dataToSend0.size(), &bytesWritten, NULL);
      //outlet.push_sample(sample4);
      sleep_for(std::chrono::milliseconds(1250 + int(uni(mt64) *250)));
  }

  autd.close();
  CloseHandle(hComm);

  return 0;
}
