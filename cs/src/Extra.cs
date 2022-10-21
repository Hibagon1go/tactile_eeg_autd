﻿/*
 * File: Extra.cs
 * Project: src
 * Created Date: 11/10/2022
 * Author: Shun Suzuki
 * -----
 * Last Modified: 21/10/2022
 * Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
 * -----
 * Copyright (c) 2022 Shun Suzuki. All rights reserved.
 * 
 */


namespace AUTD3Sharp
{
    namespace Extra
    {
        public class GeometryViewer
        {

            private int _width;
            private int _height;
            private bool _vsync;
            private int _gpuIdx;


            public GeometryViewer()
            {
                _width = 800;
                _height = 600;
                _vsync = true;
                _gpuIdx = 0;
            }

            public GeometryViewer WindowSize(int width, int height)
            {
                _width = width;
                _height = height;
                return this;
            }

            public GeometryViewer Vsync(bool vsync)
            {
                _vsync = vsync;
                return this;
            }

            public GeometryViewer GpuIdx(int idx)
            {
                _gpuIdx = idx;
                return this;
            }

            public void View(Geometry geometry)
            {
                NativeMethods.ExtraGeometryViewer.AUTDExtraGeometryViewer(geometry._cntPtr, _width, _height, _vsync, _gpuIdx);
            }
        }

        public class Simulator
        {
            private string _settingsPath;

            private bool _vsync;
            private ushort _port;
            private string _ip;
            private int _gpuIdx;

            public Simulator()
            {
                _settingsPath = "settings.json";
                _port = 50632;
                _vsync = true;
                _ip = "127.0.0.1";
                _gpuIdx = 0;
            }

            public Simulator SettingsPath(string settingsPath)
            {
                _settingsPath = settingsPath;
                return this;
            }

            public Simulator GpuIdx(int idx)
            {
                _gpuIdx = idx;
                return this;
            }

            public Simulator Vsync(bool vsync)
            {
                _vsync = vsync;
                return this;
            }

            public Simulator Port(ushort port)
            {
                _port = port;
                return this;
            }
            public Simulator Ip(string ip)
            {
                _ip = ip;
                return this;
            }

            public void Run()
            {
                NativeMethods.ExtraSimulator.AUTDExtraSimulator(_settingsPath, _port, _ip, _vsync, _gpuIdx);
            }
        }
    }
}
