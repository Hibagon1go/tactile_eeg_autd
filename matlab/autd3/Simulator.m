%{
%File: Simulator.m
%Project: autd3
%Created Date: 10/10/2022
%Author: Shun Suzuki
%-----
%Last Modified: 10/11/2022
%Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
%-----
%Copyright (c) 2022 Shun Suzuki. All rights reserved.
%
%}

classdef Simulator < handle

    properties
        ptr
    end

    methods

        function obj = Simulator()
            obj.ptr = libpointer('voidPtr', 0);
        end

        function res = build(obj)
            pp = libpointer('voidPtrPtr', obj.ptr);
            calllib('autd3capi_link_simulator', 'AUTDLinkSimulator', pp);
            res = obj;
        end

    end

end
