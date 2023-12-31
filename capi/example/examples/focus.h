// File: focus.h
// Project: examples
// Created Date: 16/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 24/10/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

void* focus(void* autd) {
  void* s = NULL;
  AUTDCreateSilencer(&s, 10, 4096);
  AUTDSend(autd, s, NULL);
  AUTDDeleteSilencer(s);

  double x = 90.0;
  double y = 70.0;
  double z = 150.0;

  void* g = NULL;
  AUTDGainFocus(&g, x, y, z, 1.0);

  void* m = NULL;
  AUTDModulationSine(&m, 150, 1.0, 0.5);

  AUTDSend(autd, m, g);

  AUTDDeleteGain(g);
  AUTDDeleteModulation(m);

  return autd;
}
