/*
 * File: cpu_defined.rs
 * Project: src
 * Created Date: 02/05/2022
 * Author: Shun Suzuki
 * -----
 * Last Modified: 05/12/2022
 * Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
 * -----
 * Copyright (c) 2022 Shun Suzuki. All rights reserved.
 *
 */

bitflags::bitflags! {
    pub struct CPUControlFlags : u8 {
        const NONE            = 0;
        const MOD             = 1 << 0;
        const MOD_BEGIN       = 1 << 1;
        const MOD_END         = 1 << 2;
        const CONFIG_EN_N     = 1 << 0;
        const CONFIG_SILENCER = 1 << 1;
        const CONFIG_SYNC     = 1 << 2;
        const WRITE_BODY      = 1 << 3;
        const STM_BEGIN       = 1 << 4;
        const STM_END         = 1 << 5;
        const IS_DUTY         = 1 << 6;
        const MOD_DELAY       = 1 << 7;
    }
}
