// File: gain.hpp
// Project: stm
// Created Date: 11/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 16/05/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Hapis Lab. All rights reserved.
//

#pragma once

#include <vector>

#include "autd3/core/geometry/normal_transducer.hpp"
#include "autd3/core/geometry/transducer.hpp"
#include "autd3/core/interface.hpp"
#include "autd3/driver/cpu/operation.hpp"
#include "stm.hpp"

namespace autd3::core {

/**
 * @brief GainSTM provides a function to display Gain sequentially and periodically.
 * @details GainSTM uses a timer on the FPGA to ensure that Gain is precisely timed.
 * GainSTM currently has the following three limitations.
 * 1. The maximum number of gains is driver::GAIN_STM_BUF_SIZE_MAX.
 */
template <typename T, std::enable_if_t<std::is_base_of_v<Transducer<typename T::D>, T>, nullptr_t> = nullptr>
struct GainSTM {};

/**
 * @brief GainSTM for LegacyTransducer
 */
template <>
struct GainSTM<LegacyTransducer> final : STM, DatagramBody<LegacyTransducer> {
  explicit GainSTM(const Geometry<LegacyTransducer>& geometry) : STM(), DatagramBody<LegacyTransducer>(), _geometry(geometry), _sent(0) {}

  /**
   * @brief Add gain
   * @param[in] gain gain
   */
  template <typename G, std::enable_if_t<std::is_base_of_v<Gain<LegacyTransducer>, G>, nullptr_t> = nullptr>
  void add(G& gain) {
    if (_gains.size() + 1 > driver::GAIN_STM_BUF_SIZE_MAX) throw std::runtime_error("PointSTM out of buffer");

    gain.build(_geometry);

    _gains.emplace_back(gain.drives());
  }

  size_t size() override { return _gains.size(); }

  void init() override { _sent = 0; }

  void pack(const uint8_t msg_id, const Geometry<LegacyTransducer>& geometry, driver::TxDatagram& tx) override {
    gain_stm_legacy_header(msg_id, tx);

    if (is_finished()) return;

    const auto is_first_frame = _sent == 0;
    const auto is_last_frame = _sent + 1 == _gains.size() + 1;

    if (is_first_frame) {
      gain_stm_legacy_body(nullptr, is_first_frame, _freq_div, is_last_frame, tx);
      _sent += 1;
      return;
    }

    gain_stm_legacy_body(_gains.at(_sent - 1).data.data(), is_first_frame, _freq_div, is_last_frame, tx);
    _sent += 1;
  }

  [[nodiscard]] bool is_finished() const override { return _sent == _gains.size() + 1; }

 private:
  const Geometry<LegacyTransducer>& _geometry;
  std::vector<LegacyTransducer::D> _gains;
  size_t _sent;
};

/**
 * @brief GainSTM for NormalTransducer
 */
template <>
struct GainSTM<NormalTransducer> final : STM, DatagramBody<NormalTransducer> {
  explicit GainSTM(const Geometry<NormalTransducer>& geometry)
      : STM(), DatagramBody<NormalTransducer>(), _geometry(geometry), _sent(0), _next_duty(false) {}

  /**
   * @brief Add gain
   * @param[in] gain gain
   */
  template <typename G, std::enable_if_t<std::is_base_of_v<Gain<NormalTransducer>, G>, nullptr_t> = nullptr>
  void add(G& gain) {
    if (_gains.size() + 1 > driver::GAIN_STM_BUF_SIZE_MAX) throw std::runtime_error("PointSTM out of buffer");

    gain.build(_geometry);

    _gains.emplace_back(gain.drives());
  }

  size_t size() override { return _gains.size(); }

  void init() override { _sent = 0; }

  void pack(const uint8_t msg_id, const Geometry<NormalTransducer>& geometry, driver::TxDatagram& tx) override {
    gain_stm_normal_header(msg_id, tx);

    if (is_finished()) return;

    const auto is_first_frame = _sent == 0;
    const auto is_last_frame = _sent + 1 == _gains.size() * 2 + 1;

    if (is_first_frame) {
      gain_stm_normal_phase(nullptr, is_first_frame, _freq_div, tx);
      _sent += 1;
      return;
    }

    if (!_next_duty)
      gain_stm_normal_phase(_gains.at((_sent - 1) / 2).phases.data(), is_first_frame, _freq_div, tx);
    else
      gain_stm_normal_duty(_gains.at((_sent - 1) / 2).duties.data(), is_last_frame, tx);

    _next_duty = !_next_duty;

    _sent += 1;
  }

  [[nodiscard]] bool is_finished() const override { return _sent == _gains.size() * 2 + 1; }

 private:
  const Geometry<NormalTransducer>& _geometry;
  std::vector<NormalTransducer::D> _gains;
  size_t _sent;
  bool _next_duty;
};

}  // namespace autd3::core