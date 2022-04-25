// File: param.h
// Project: inc
// Created Date: 22/04/2022
// Author: Shun Suzuki
// -----
// Last Modified: 25/04/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Hapis Lab. All rights reserved.
//

#ifndef INC_PARAMS_H_
#define INC_PARAMS_H_

#define BRAM_SELECT_CONTROLLER (0x0);
#define BRAM_SELECT_MOD (0x1);
#define BRAM_SELECT_NORMAL (0x2);
#define BRAM_SELECT_STM (0x3);

#define BRAM_ADDR_CTL_REG (0x000);
#define BRAM_ADDR_FPGA_INFO (0x001);
#define BRAM_ADDR_EC_SYNC_CYCLE_TICKS (0x010);
#define BRAM_ADDR_EC_SYNC_TIME_0 (ADDR_EC_SYNC_CYCLE_TICKS + 1);
#define BRAM_ADDR_EC_SYNC_TIME_1 (ADDR_EC_SYNC_CYCLE_TICKS + 2);
#define BRAM_ADDR_EC_SYNC_TIME_2 (ADDR_EC_SYNC_CYCLE_TICKS + 3);
#define BRAM_ADDR_EC_SYNC_TIME_3 (ADDR_EC_SYNC_CYCLE_TICKS + 4);
#define BRAM_ADDR_MOD_ADDR_OFFSET (0x020);
#define BRAM_ADDR_MOD_CYCLE (0x021);
#define BRAM_ADDR_MOD_FREQ_DIV_0 (0x022);
#define BRAM_ADDR_MOD_FREQ_DIV_1 (0x023);
#define BRAM_ADDR_VERSION_NUM (0x03F);  // For backward compatibility
#define BRAM_ADDR_SILENT_CYCLE (0x040);
#define BRAM_ADDR_SILENT_STEP (0x041);
#define BRAM_ADDR_STM_ADDR_OFFSET (0x050);
#define BRAM_ADDR_STM_CYCLE (0x051);
#define BRAM_ADDR_STM_FREQ_DIV_0 (0x052);
#define BRAM_ADDR_STM_FREQ_DIV_1 (0x053);
#define BRAM_ADDR_SOUND_SPEED_0 (0x054);
#define BRAM_ADDR_SOUND_SPEED_1 (0x055);
#define BRAM_ADDR_CYCLE_BASE (0x100);

#define CTL_REG_LEGACY_MODE_BIT (0);
#define CTL_REG_FORCE_FAN_BIT (4);
#define CTL_REG_OP_MODE_BIT (5);
#define CTL_REG_STM_GAIN_MODE_BIT (6);
#define CTL_REG_SYNC_BIT (14);
#define CTL_REG_WDT_RST_BIT (15);

#endif  // INC_PARAMS_H_
