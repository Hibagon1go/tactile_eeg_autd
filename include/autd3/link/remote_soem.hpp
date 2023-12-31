// File: remote_soem.hpp
// Project: link
// Created Date: 26/10/2022
// Author: Shun Suzuki
// -----
// Last Modified: 02/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#include <memory>
#include <string>

#include "autd3/core/link.hpp"

namespace autd3::link {

/**
 * @brief Link using SOEMAUTDServer
 */
class RemoteSOEM {
 public:
  /**
   * @brief Create RemoteSOEM
   */
  core::LinkPtr build();

  RemoteSOEM& ip(const std::string& ip) {
    _ip = ip;
    return *this;
  }

  RemoteSOEM& port(const uint16_t& port) {
    _port = port;
    return *this;
  }

  /**
   * @brief Constructor
   */
  RemoteSOEM() : _port(50632) {}
  ~RemoteSOEM() = default;
  RemoteSOEM(const RemoteSOEM& v) noexcept = delete;
  RemoteSOEM& operator=(const RemoteSOEM& obj) = delete;
  RemoteSOEM(RemoteSOEM&& obj) = delete;
  RemoteSOEM& operator=(RemoteSOEM&& obj) = delete;

 private:
  std::string _ip;
  uint16_t _port;
};
}  // namespace autd3::link
