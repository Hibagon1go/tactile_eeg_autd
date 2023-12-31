﻿/*
 * File: Link.cs
 * Project: src
 * Created Date: 28/04/2021
 * Author: Shun Suzuki
 * -----
 * Last Modified: 09/12/2022
 * Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
 * -----
 * Copyright (c) 2021 Shun Suzuki. All rights reserved.
 * 
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using Microsoft.Win32.SafeHandles;

#if UNITY_2020_2_OR_NEWER
#nullable enable
#endif

namespace AUTD3Sharp
{
    namespace Link
    {
        [ComVisible(false)]
        public class Link : SafeHandleZeroOrMinusOneIsInvalid
        {
            internal IntPtr LinkPtr => handle;

            internal Link(IntPtr handle) : base(false)
            {
                SetHandle(handle);
            }

            protected override bool ReleaseHandle() => true;
        }

        public sealed class Bundle
        {
            private List<Link> _links;

            public Bundle(Link link)
            {
                _links = new List<Link>();
                _links.Add(link);
            }

            public Bundle Link(Link link)
            {
                _links.Add(link);
                return this;
            }

            public Link Build()
            {
                var n = _links.Count;
                var links = new IntPtr[n];
                for (var i = 0; i < n; i++)
                    links[i] = _links[i].LinkPtr;
                NativeMethods.LinkBundle.AUTDLinkBundle(out var handle, links, n);
                return new Link(handle);
            }
        }

        public sealed class Debug
        {
            public Link Build()
            {
                NativeMethods.LinkDebug.AUTDLinkDebug(out var handle);
                return new Link(handle);
            }
        }


        public sealed class SOEM
        {
            [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, BestFitMapping = false, ThrowOnUnmappableChar = true)] public delegate void OnLostCallbackDelegate(string str);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, BestFitMapping = false, ThrowOnUnmappableChar = true)] public delegate void OnLogOutputCallback(string str);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)] public delegate void OnLogFlushCallback();

            private string _ifname;
            private ushort _sendCycle;
            private ushort _sync0Cycle;
            private bool _freerun;
            private bool _highPrecision;
            private IntPtr _onLost;
            private ulong _checkInterval;

            public SOEM()
            {
                _ifname = "";
                _sendCycle = 2;
                _sync0Cycle = 2;
                _freerun = false;
                _highPrecision = false;
                _onLost = IntPtr.Zero;
                _checkInterval = 500;
            }

            public SOEM Ifname(string ifname)
            {
                _ifname = ifname;
                return this;
            }

            public SOEM SendCycle(ushort sendCycle)
            {
                _sendCycle = sendCycle;
                return this;
            }

            public SOEM Sync0Cycle(ushort sync0Cycle)
            {
                _sync0Cycle = sync0Cycle;
                return this;
            }

            public SOEM FreeRun(bool freerun)
            {
                _freerun = freerun;
                return this;
            }

            public SOEM HighPrecision(bool highPrecision)
            {
                _highPrecision = highPrecision;
                return this;
            }

            public SOEM OnLost(OnLostCallbackDelegate onLost)
            {
                _onLost = Marshal.GetFunctionPointerForDelegate(onLost);
                return this;
            }
            public SOEM CheckInterval(ulong interval)
            {
                _checkInterval = interval;
                return this;
            }

            public Link Build()
            {
                NativeMethods.LinkSOEM.AUTDLinkSOEM(out var handle, _ifname, _sync0Cycle, _sendCycle, _freerun, _onLost, _highPrecision, _checkInterval);
                return new Link(handle);
            }

            public static IEnumerable<EtherCATAdapter> EnumerateAdapters()
            {
                var size = NativeMethods.LinkSOEM.AUTDGetAdapterPointer(out var handle);
                for (var i = 0; i < size; i++)
                {
                    var sbDesc = new StringBuilder(128);
                    var sbName = new StringBuilder(128);
                    NativeMethods.LinkSOEM.AUTDGetAdapter(handle, i, sbDesc, sbName);
                    yield return new EtherCATAdapter(sbDesc.ToString(), sbName.ToString());
                }
                NativeMethods.LinkSOEM.AUTDFreeAdapterPointer(handle);
            }

            public static void SetLogLevel(int level)
            {
                NativeMethods.LinkSOEM.AUTDLinkSOEMSetLogLevel(level);
            }

            public static void SetLogFunc(OnLogOutputCallback output, OnLogFlushCallback flush)
            {
                var onOutput = Marshal.GetFunctionPointerForDelegate(output);
                var onFlush = Marshal.GetFunctionPointerForDelegate(flush);

                NativeMethods.LinkSOEM.AUTDLinkSOEMSetDefaultLogger(onOutput, onFlush);
            }
        }

        public sealed class RemoteSOEM
        {
            private string _ip;
            private ushort _port;

            public RemoteSOEM()
            {
                _ip = "";
                _port = 50632;
            }

            public RemoteSOEM Ip(string ip)
            {
                _ip = ip;
                return this;
            }

            public RemoteSOEM Port(ushort port)
            {
                _port = port;
                return this;
            }

            public Link Build()
            {
                NativeMethods.LinkRemoteSOEM.AUTDLinkRemoteSOEM(out var handle, _ip, _port);
                return new Link(handle);
            }
        }

        public sealed class TwinCAT
        {
            public Link Build()
            {
                NativeMethods.LinkTwinCAT.AUTDLinkTwinCAT(out var handle);
                return new Link(handle);
            }
        }

        public sealed class RemoteTwinCAT
        {
            private readonly string _remoteAmsNetId;
            private string _remoteIp;
            private string _localAmsNetId;

            public RemoteTwinCAT(string remoteAmsNetId)
            {
                _remoteAmsNetId = remoteAmsNetId;
                _localAmsNetId = string.Empty;
                _remoteIp = string.Empty;
            }

            public RemoteTwinCAT RemoteIp(string remoteIp)
            {
                _remoteIp = remoteIp;
                return this;
            }

            public RemoteTwinCAT LocalAmsNetId(string localAmsNetId)
            {
                _localAmsNetId = localAmsNetId;
                return this;
            }

            public Link Build()
            {
                NativeMethods.LinkRemoteTwinCAT.AUTDLinkRemoteTwinCAT(out var handle, _remoteIp, _remoteAmsNetId, _localAmsNetId);
                return new Link(handle);
            }
        }

        public sealed class Simulator
        {
            public Link Build()
            {
                NativeMethods.LinkSimulator.AUTDLinkSimulator(out var handle);
                return new Link(handle);
            }
        }

        public readonly struct EtherCATAdapter : IEquatable<EtherCATAdapter>
        {
            public string Desc { get; }
            public string Name { get; }

            internal EtherCATAdapter(string desc, string name)
            {
                Desc = desc;
                Name = name;
            }

            public override string ToString() => $"{Desc}, {Name}";
            public bool Equals(EtherCATAdapter other) => Desc.Equals(other.Desc) && Name.Equals(other.Name);
            public static bool operator ==(EtherCATAdapter left, EtherCATAdapter right) => left.Equals(right);
            public static bool operator !=(EtherCATAdapter left, EtherCATAdapter right) => !left.Equals(right);
            public override bool Equals(object? obj) => obj is EtherCATAdapter adapter && Equals(adapter);
            public override int GetHashCode() => Desc.GetHashCode() ^ Name.GetHashCode();
        }
    }
}

#if UNITY_2020_2_OR_NEWER
#nullable disable
#endif
