// File: gain_stm.h
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

#include <math.h>

#define DUTY_PHASE_FULL (0x0001)
#define PHASE_FULL (0x0002)
#define PHASE_HALF (0x0004)

void* gain_stm(void* autd) {
  void* s = NULL;
  AUTDCreateSilencer(&s, 0xFFFF, 4096);
  AUTDSend(autd, s, NULL);
  AUTDDeleteSilencer(s);

  void* m = NULL;
  AUTDModulationStatic(&m, 0xFF);
  AUTDSend(autd, m, NULL);

  double x = 90.0;
  double y = 70.0;
  double z = 150.0;

  void* stm = NULL;
  AUTDGainSTM(&stm, autd);

  const int32_t point_num = 200;
  for (int32_t i = 0; i < point_num; i++) {
    const double radius = 30.0;
    const double theta = 2.0 * M_PI * (double)i / (double)point_num;
    void* g = NULL;
    AUTDGainFocus(&g, x + radius * cos(theta), y + radius * sin(theta), z, 1.0);
    AUTDGainSTMAdd(stm, g);
    AUTDDeleteGain(g);
  }

  const uint32_t v = AUTDSTMSamplingFrequencyDivision(stm);
  printf("Sample frequency division is %d\n", v);

  const double actual_freq = AUTDSTMSetFrequency(stm, 1.0);
  printf("Actual frequency is %lf Hz\n", actual_freq);

  AUTDSend(autd, NULL, stm);

  AUTDDeleteSTM(stm);
  AUTDDeleteModulation(m);

  return autd;
}
