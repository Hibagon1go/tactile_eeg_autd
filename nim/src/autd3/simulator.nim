# File: simulator.nim
# Project: autd3
# Created Date: 08/08/2022
# Author: Shun Suzuki
# -----
# Last Modified: 10/11/2022
# Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
# -----
# Copyright (c) 2022 Shun Suzuki. All rights reserved.
#



import native_methods/autd3capi_link_simulator
import link

type Simulator* = object of RootObj

proc initSimulator*(): Simulator =
    discard

func build*(cnt: Simulator): Link =
    AUTDLinkSimulator(result.p.addr)
