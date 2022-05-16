// File: legacy_transducer.hpp
// Project: geometry
// Created Date: 11/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 16/05/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Hapis Lab. All rights reserved.
//

#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "autd3/driver/cpu/operation.hpp"
#include "autd3/driver/fpga/defined.hpp"
#include "transducer.hpp"

namespace autd3::core {

/**
 * @brief DriveData for LegacyTransducer
 */
template <typename T>
struct LegacyDriveData final : DriveData<T> {
  void init(const size_t size) override { data.resize(size, driver::LegacyDrive{0x00, 0x00}); }
  void set_drive(const T& tr, const double phase, const double amp) noexcept override {
    data.at(tr.id()).duty = static_cast<uint8_t>(std::round(510.0 * std::asin(amp) / driver::pi));
    data.at(tr.id()).phase = static_cast<uint8_t>(static_cast<int32_t>(std::round(phase * 256.0)) & 0xFF);
  }

  void copy_from(size_t idx, const typename T::D& src) override {
    const auto* s = src.data.data() + idx * driver::NUM_TRANS_IN_UNIT;
    auto* d = data.data() + idx * driver::NUM_TRANS_IN_UNIT;
    std::memcpy(d, s, sizeof(driver::LegacyDrive) * driver::NUM_TRANS_IN_UNIT);
  }

  std::vector<driver::LegacyDrive> data{};
};

/**
 * \brief Transduce with 40kHz frequency
 */
struct LegacyTransducer final : Transducer<LegacyDriveData<LegacyTransducer>> {
  LegacyTransducer(const size_t id, Vector3 pos, Vector3 x_direction, Vector3 y_direction, Vector3 z_direction) noexcept
      : Transducer(id, std::move(pos), std::move(x_direction), std::move(y_direction), std::move(z_direction)) {}
  ~LegacyTransducer() override = default;
  LegacyTransducer(const LegacyTransducer& v) noexcept = default;
  LegacyTransducer& operator=(const LegacyTransducer& obj) = default;
  LegacyTransducer(LegacyTransducer&& obj) = default;
  LegacyTransducer& operator=(LegacyTransducer&& obj) = default;

  [[nodiscard]] uint16_t cycle() const noexcept override { return 4096; }
  [[nodiscard]] double frequency() const noexcept override { return 40e3; }
  [[nodiscard]] double wavelength(const double sound_speed) const noexcept override { return sound_speed * 1e3 / 40e3; }
  [[nodiscard]] double wavenumber(const double sound_speed) const noexcept override { return 2.0 * driver::pi * 40e3 / (sound_speed * 1e3); }

  static void pack_header(const uint8_t msg_id, driver::TxDatagram& tx) noexcept { normal_legacy_header(msg_id, tx); }

  static void pack_body(bool& phase_sent, bool& duty_sent, const D& drives, driver::TxDatagram& tx) noexcept {
    normal_legacy_body(drives.data.data(), tx);
    phase_sent = true;
    duty_sent = true;
  }
};

}  // namespace autd3::core