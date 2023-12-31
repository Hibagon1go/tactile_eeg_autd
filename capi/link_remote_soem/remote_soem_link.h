﻿// File: remote_soem_link.h
// Project: link_remote_soem
// Created Date: 03/11/2022
// Author: Shun Suzuki
// -----
// Last Modified: 03/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#include "../base/header.hpp"

#ifdef __cplusplus
extern "C" {
#endif
EXPORT_AUTD void AUTDLinkRemoteSOEM(OUT void** out, IN const char* ip, IN uint16_t port);
#ifdef __cplusplus
}
#endif
