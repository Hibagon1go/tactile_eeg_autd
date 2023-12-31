// File: gain_stm.hpp
// Project: tests
// Created Date: 11/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 13/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

/*
#pragma once

#include <autd3.hpp>
#include <vector>

inline void gain_stm(autd3::Controller& autd) {
  auto config = autd3::SilencerConfig::none();

  autd3::modulation::Static m;

  autd3::GainSTM stm(autd.geometry());
  // stm.mode() = autd3::GainSTMMode::PhaseFull;

  const autd3::Vector3 center = autd.geometry().center() + autd3::Vector3(0.0, 0.0, 150.0);
  constexpr size_t points_num = 50;
  constexpr auto radius = 30.0;
  std::vector<size_t> points(points_num);
  std::iota(points.begin(), points.end(), 0);
  std::for_each(points.begin(), points.end(), [&](const size_t i) {
    const auto theta = 2.0 * autd3::pi * static_cast<double>(i) / static_cast<double>(points_num);
    autd3::gain::Focus g(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0.0));
    stm.add(g);
  });

  const auto actual_freq = stm.set_frequency(1);
  std::cout << "Actual frequency is " << actual_freq << " Hz\n";
  autd << config << m, stm;
}*/

// File: focus.hpp
// Project: tests
// Created Date: 11/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 17/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#include "autd3.hpp"

inline void gain_stm(autd3::Controller& autd) {
    autd3::modulation::Sine m(125);  // 150Hz AM

    const autd3::Vector3 center = autd3::Vector3(80.0, 80.0, 180.0);
    autd3::gain::Focus g(center);

    autd << autd3::SilencerConfig::none() << m, g;
}
