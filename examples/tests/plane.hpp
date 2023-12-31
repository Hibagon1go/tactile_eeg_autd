// File: plane.hpp
// Project: tests
// Created Date: 16/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 17/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#include "autd3.hpp"

inline void plane_test(autd3::Controller& autd) {
  autd3::SilencerConfig silencer;

  autd3::modulation::Sine m(150);  // 150Hz AM

  const autd3::Vector3 direction = autd3::Vector3::UnitZ();
  autd3::gain::PlaneWave g(direction);

  autd << silencer << m, g;
}
