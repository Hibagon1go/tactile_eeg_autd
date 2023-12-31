// File: geometry.hpp
// Project: geometry
// Created Date: 16/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 30/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#include <numeric>
#include <vector>

#include "transducer.hpp"

namespace autd3::core {

/**
 * \brief Device consists of transducers
 */
struct Device {
  Device() = default;
  virtual ~Device() = default;
  Device(const Device& v) noexcept = default;
  Device& operator=(const Device& obj) = default;
  Device(Device&& obj) = default;
  Device& operator=(Device&& obj) = default;

  [[nodiscard]] virtual std::vector<Transducer> get_transducers(size_t start_id) const = 0;
};

/**
 * @brief Geometry of all transducers
 */
struct Geometry {
  Geometry() = default;

  /**
   * @brief Number of devices
   */
  [[nodiscard]] size_t num_devices() const noexcept { return _device_map.size(); }

  /**
   * @brief Number of transducers
   */
  [[nodiscard]] size_t num_transducers() const noexcept { return _transducers.size(); }

  /**
   * @brief Center position of all transducers
   */
  [[nodiscard]] Vector3 center() const {
    const Vector3 zero = Vector3::Zero();
    if (num_transducers() == 0) return Vector3::Zero();
    return std::accumulate(begin(), end(), zero,
                           [](const Vector3& acc, const Transducer& tr) {
                             Vector3 res = acc + tr.position();
                             return res;
                           }) /
           _transducers.size();
  }

  /**
   * @brief Add device to Geometry
   * @tparam T Class inheriting from Device
   * @param device device
   */
  template <typename T>
  auto add_device(T&& device) -> std::enable_if_t<std::is_base_of_v<Device, T>> {
    const auto id = _transducers.size();
    const auto transducers = device.get_transducers(id);
    if (transducers.size() > 256) throw std::runtime_error("The maximum number of transducers per device is 256.");
    _transducers.insert(_transducers.end(), transducers.begin(), transducers.end());
    _device_map.emplace_back(transducers.size());
  }

  /**
   * @return device_map contains the number of transducers each device has
   */
  [[nodiscard]] const std::vector<size_t>& device_map() const noexcept { return _device_map; }

  [[nodiscard]] std::vector<Transducer>::const_iterator begin() const noexcept { return _transducers.begin(); }
  [[nodiscard]] std::vector<Transducer>::const_iterator end() const noexcept { return _transducers.end(); }
  [[nodiscard]] std::vector<Transducer>::iterator begin() noexcept { return _transducers.begin(); }
  [[nodiscard]] std::vector<Transducer>::iterator end() noexcept { return _transducers.end(); }
  [[nodiscard]] const Transducer& operator[](const size_t i) const { return _transducers[i]; }
  [[nodiscard]] Transducer& operator[](const size_t i) { return _transducers[i]; }

 private:
  std::vector<Transducer> _transducers;
  std::vector<size_t> _device_map;
};

}  // namespace autd3::core
