'''
File: focus.py
Project: samples
Created Date: 24/05/2021
Author: Shun Suzuki
-----
Last Modified: 21/10/2022
Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
-----
Copyright (c) 2022 Shun Suzuki. All rights reserved.

'''


from pyautd3 import Controller, SilencerConfig
from pyautd3.gain import Focus
from pyautd3.modulation import Sine
import numpy as np


def simple(autd: Controller):
    config = SilencerConfig()
    autd.send(config)

    f = Focus(autd.geometry.center + np.array([0., 0., 150.]))
    m = Sine(150)

    autd.send(m, f)
