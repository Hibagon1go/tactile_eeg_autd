%{
%File: Constraint.m
%Project: autd3
%Created Date: 10/06/2022
%Author: Shun Suzuki
%-----
%Last Modified: 10/11/2022
%Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
%-----
%Copyright (c) 2022 Shun Suzuki. All rights reserved.
%
%}

classdef Constraint < handle

    properties
        ptr
    end

    methods

        function obj = Constraint()
            obj.ptr = libpointer('voidPtr', 0);
        end

    end

end
