# File: emulator.nim
# Project: src
# Created Date: 13/06/2022
# Author: Shun Suzuki
# -----
# Last Modified: 10/10/2022
# Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
# -----
# Copyright (c) 2022 Shun Suzuki. All rights reserved.
#


import autd3
import autd3/simulator
import tests/runner

when isMainModule:
    try:
        var cnt = initController()
        cnt.addDevice([0.0, 0.0, 0.0], [0.0, 0.0, 0.0])

        var simulator = initSimulator()
        let link = simulator.port(50632).build()
        if not cnt.open(link):
            echo Controller.lastError
            quit(-1)

        cnt.checkTrials = 50

        run(cnt)

    except:
        let
            e = getCurrentException()
            msg = getCurrentExceptionMsg()
        echo "Got exception ", repr(e), " with message ", msg