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

inline void am_single_100(autd3::Controller &autd)
{
  autd3::modulation::Sine m(100);

  const autd3::Vector3 center = autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH - 10.0, -10.0, 240.0);
  autd3::gain::Focus g(center, 1.0);

  autd3::SilencerConfig config;
  config.step = 2750;

  autd << config << m, g;
}

inline void am_single_200(autd3::Controller& autd)
{
	autd3::modulation::Sine m(200);

	const autd3::Vector3 center = autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH - 10.0, -10.0, 240.0);
	autd3::gain::Focus g(center, 1.0);

	autd3::SilencerConfig config;
	config.step = 2750;

	autd << config << m, g;
}

