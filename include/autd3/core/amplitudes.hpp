// File: amplitudes.hpp
// Project: core
// Created Date: 28/06/2022
// Author: Shun Suzuki
// -----
// Last Modified: 07/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#include <algorithm>
#include <vector>

namespace autd3::core {
/**
 * @brief Amplitude configuration for NormalPhaseMode.
 */
class Amplitudes final : public DatagramBody {
 public:
  explicit Amplitudes(const double amp = 1.0) : _amp(amp), _sent(false) {}
  ~Amplitudes() override = default;
  Amplitudes(const Amplitudes& v) = default;
  Amplitudes& operator=(const Amplitudes& obj) = default;
  Amplitudes(Amplitudes&& obj) = default;
  Amplitudes& operator=(Amplitudes&& obj) = default;

  void init() override { _sent = false; }

  void pack(const Geometry& geometry, driver::TxDatagram& tx) override {
    normal_header(tx);
    if (is_finished()) return;

    std::vector<driver::Drive> drives;
    drives.reserve(geometry.num_transducers());
    std::for_each(geometry.begin(), geometry.end(), [&](const auto& dev) {
      std::transform(dev.begin(), dev.end(), std::back_inserter(drives), [&](const auto& tr) { return driver::Drive{0.0, _amp, tr.cycle()}; });
    });

    normal_duty_body(drives, tx);
    _sent = true;
  }

  [[nodiscard]] bool is_finished() const noexcept override { return _sent; }

 private:
  double _amp;
  bool _sent;
};

}  // namespace autd3::core
