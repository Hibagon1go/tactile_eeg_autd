// File: plane.h
// Project: examples
// Created Date: 16/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 21/05/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Hapis Lab. All rights reserved.
//

#pragma once

void plane(void* autd) {
  void* s = NULL;
  AUTDCreateSilencer(&s, 10, 4096);
  AUTDSendHeader(autd, s);
  AUTDDeleteSilencer(s);

  void* g = NULL;
  AUTDGainPlaneWave(&g, 0, 0, 1, 1.0);

  void* m = NULL;
  AUTDModulationSine(&m, 150, 1.0, 0.5);

  AUTDSendHeaderBody(autd, m, g);

  AUTDDeleteGain(g);
  AUTDDeleteModulation(m);
}