// File: silencer_config.hpp
// Project: autd3
// Created Date: 11/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 16/05/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Hapis Lab. All rights reserved.
//

#pragma once

#include <cstdint>

namespace autd3 {

/**
 * @brief Silencer Configuration
 */
struct SilencerConfig {
  SilencerConfig() noexcept : SilencerConfig(10, 4096) {}
  explicit SilencerConfig(const uint16_t step, const uint16_t cycle) noexcept : step(step), cycle(cycle) {}

  /**
   * @brief Create SilencerConfig to disable Silencer
   */
  static SilencerConfig none() noexcept { return SilencerConfig(0xFFFF, 4096); }

  /**
   * @brief Silencer update step.
   * @details The smaller the step, the stronger the effect of noise reduction.
   */
  uint16_t step;
  /**
   * @brief Silencer sampling frequency division ratio.
   * @details The sampling frequency will be driver::FPGA_CLK_FREQ/cycle. The larger the cycle, the stronger the effect of noise reduction.
   */
  uint16_t cycle;
};

}  // namespace autd3