# File: Plane.jl
# Project: src
# Created Date: 14/06/2022
# Author: Shun Suzuki
# -----
# Last Modified: 14/06/2022
# Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
# -----
# Copyright (c) 2022 Shun Suzuki. All rights reserved.
# 

using StaticArrays

struct PlaneWave
    _gain::Gain
    _body_ptr
    function PlaneWave(dir::SVector{3,Float64}; amp::Float64=1.0)
        x, y, z = dir
        chandle = Ref(Ptr{Cvoid}(0))
        autd3capi.autd_gain_plane_wave(chandle, x, y, z, amp)
        g = Gain(chandle[])
        new(g, chandle[])
    end
end
