// File: utils.hpp
// Project: core
// Created Date: 20/07/2022
// Author: Shun Suzuki
// -----
// Last Modified: 07/09/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace autd3::core {

inline void spin_loop_hint() {
#if defined(_MSC_VER)
  _mm_pause();
#elif defined(__x86_64__)
  __asm__ __volatile__("pause;");
#elif defined(i386) || defined(__i386__) || defined(__i386)
  __asm__ __volatile__("pause;");
#endif
}

}  // namespace autd3::core
