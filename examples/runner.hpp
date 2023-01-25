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
#include "tests/mod_audio_file.hpp"
#endif
#include "tests/focus_stm.hpp"

#include <chrono>
#include <thread>
#include<iostream>
#include <random>

using std::this_thread::sleep_for;

inline int run(autd3::Controller& autd) {
  using F = std::function<void(autd3::Controller&)>;
  std::vector<std::pair<F, std::string>> tests = {
      std::pair(F{focus_test}, "Single focus Test"),
      std::pair(F{bessel_test}, "Bessel beam Test"),
      std::pair(F{plane_test}, "Plane wave Test"),
#ifdef BUILD_MODULATION_AUDIO_FILE
      std::pair(F{mod_audio_file_test}, "Wav and RawPCM modulation Test"),
#endif
      std::pair(F{focus_stm}, "FocusSTM Test"),
      std::pair(F{gain_stm}, "GainSTM Test"),
      std::pair(F{soft_stm}, "Software STM Test"),
#ifdef BUILD_GAIN_HOLO
      std::pair(F{holo_test}, "Holo Test"),
#endif
      std::pair(F{advanced_test}, "Custom Gain & Modulation Test"),
      std::pair(F{flag_test}, "Flag Test"),
  };
  if (autd.geometry().num_devices() == 2) tests.emplace_back(std::pair(F{group_test}, "Grouped Gain Test"));

  autd.set_sound_speed(340.0e3);

  const auto firm_infos = autd.firmware_infos();
  if (firm_infos.empty()) throw std::runtime_error("Cannot read firmware information.");
  std::cout << "================================== AUTD3 firmware information ==================================" << std::endl;
  std::copy(firm_infos.begin(), firm_infos.end(), std::ostream_iterator<autd3::FirmwareInfo>(std::cout, "\n"));
  std::cout << "================================================================================================" << std::endl;

  // ÉgÉäÉKÅ[èÄîı
  lsl::stream_info info("LSLMarkersInletStreamName1", "Markers", 32, 100, lsl::cf_float32, "NIC");
  lsl::stream_outlet outlet(info, 0, 360);
  std::vector<float> sample(32, 1.0);

  autd << autd3::clear << autd3::synchronize;

  std::mt19937_64 mt64(0);  //Ç‡Ç∆Ç…Ç∑ÇÈêÆêîå^ÇÃóêêîÇÕï ìrïKóv
  std::uniform_real_distribution<double> uni(0, 1);

  while (true) {
      //20msèoÇ∑->2-3sé~ÇﬂÇÈ
      outlet.push_sample(sample);
      tests[0].first(autd);
      sleep_for(std::chrono::milliseconds(20));
      autd << autd3::stop;
      sleep_for(std::chrono::milliseconds(2000 + int(uni(mt64)*1000)));
  }

  autd.close();

  return 0;
}
