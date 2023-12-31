/*
 * File: controller.rs
 * Project: src
 * Created Date: 27/04/2022
 * Author: Shun Suzuki
 * -----
 * Last Modified: 07/12/2022
 * Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
 * -----
 * Copyright (c) 2022 Shun Suzuki. All rights reserved.
 *
 */

use std::{
    marker::PhantomData,
    sync::atomic::{self, AtomicU8},
};

use anyhow::{Ok, Result};

use autd3_core::{
    amplitude::Amplitudes,
    clear::Clear,
    datagram::{DatagramBody, DatagramHeader, Empty, Filled, NullBody, NullHeader, Sendable},
    geometry::{Geometry, Transducer},
    link::Link,
    FirmwareInfo, RxDatagram, TxDatagram, MSG_BEGIN, MSG_END,
};

static MSG_ID: AtomicU8 = AtomicU8::new(MSG_BEGIN);

pub struct Sender<'a, 'b, L: Link, T: Transducer, S: Sendable<T>, H, B> {
    cnt: &'a mut Controller<L, T>,
    buf: &'b mut S,
    sent: bool,
    _head: PhantomData<H>,
    _body: PhantomData<B>,
}

impl<'a, 'b, L: Link, T: Transducer, S: Sendable<T>> Sender<'a, 'b, L, T, S, Empty, Empty> {
    pub fn new(cnt: &'a mut Controller<L, T>, s: &'b mut S) -> Sender<'a, 'b, L, T, S, S::H, S::B> {
        Sender {
            cnt,
            buf: s,
            sent: false,
            _head: PhantomData,
            _body: PhantomData,
        }
    }
}

impl<'a, 'b, L: Link, T: Transducer, S: Sendable<T>> Sender<'a, 'b, L, T, S, Filled, Empty> {
    pub fn send<B: DatagramBody<T>>(mut self, b: &'b mut B) -> Result<bool> {
        self.buf.init()?;
        b.init()?;

        autd3_core::force_fan(&mut self.cnt.tx_buf, self.cnt.force_fan);
        autd3_core::reads_fpga_info(&mut self.cnt.tx_buf, self.cnt.reads_fpga_info);

        loop {
            let msg_id = self.cnt.get_id();
            self.buf
                .pack(msg_id, &self.cnt.geometry, &mut self.cnt.tx_buf)?;
            b.pack(&self.cnt.geometry, &mut self.cnt.tx_buf)?;
            self.cnt.link.send(&self.cnt.tx_buf)?;
            let success = self.cnt.wait_msg_processed(self.cnt.ack_check_timeout)?;
            if !self.cnt.ack_check_timeout.is_zero() && !success {
                self.sent = true;
                return Ok(false);
            }
            if self.buf.is_finished() && b.is_finished() {
                break;
            }
            if self.cnt.ack_check_timeout.is_zero() {
                std::thread::sleep(self.cnt.send_interval);
            }
        }
        self.sent = true;
        Ok(true)
    }

    pub fn flush(self) -> Result<bool> {
        let mut b = NullBody::new();
        self.send(&mut b)
    }
}

impl<'a, 'b, L: Link, T: Transducer, S: Sendable<T>> Sender<'a, 'b, L, T, S, Empty, Filled> {
    pub fn send<H: DatagramHeader>(mut self, b: &'b mut H) -> Result<bool> {
        b.init()?;
        self.buf.init()?;

        autd3_core::force_fan(&mut self.cnt.tx_buf, self.cnt.force_fan);
        autd3_core::reads_fpga_info(&mut self.cnt.tx_buf, self.cnt.reads_fpga_info);

        loop {
            let msg_id = self.cnt.get_id();
            b.pack(msg_id, &mut self.cnt.tx_buf)?;
            self.buf
                .pack(msg_id, &self.cnt.geometry, &mut self.cnt.tx_buf)?;
            self.cnt.link.send(&self.cnt.tx_buf)?;
            let success = self.cnt.wait_msg_processed(self.cnt.ack_check_timeout)?;
            if !self.cnt.ack_check_timeout.is_zero() && !success {
                self.sent = true;
                return Ok(false);
            }
            if self.buf.is_finished() && b.is_finished() {
                break;
            }
            if self.cnt.ack_check_timeout.is_zero() {
                std::thread::sleep(self.cnt.send_interval);
            }
        }
        self.sent = true;
        Ok(true)
    }

    pub fn flush(self) -> Result<bool> {
        let mut h = NullHeader::new();
        self.send(&mut h)
    }
}

impl<'a, 'b, L: Link, T: Transducer, S: Sendable<T>, H, B> Drop for Sender<'a, 'b, L, T, S, H, B> {
    fn drop(&mut self) {
        if !self.sent {
            if self.buf.init().is_err() {
                return;
            }

            autd3_core::force_fan(&mut self.cnt.tx_buf, self.cnt.force_fan);
            autd3_core::reads_fpga_info(&mut self.cnt.tx_buf, self.cnt.reads_fpga_info);

            loop {
                let msg_id = self.cnt.get_id();
                if self
                    .buf
                    .pack(msg_id, &self.cnt.geometry, &mut self.cnt.tx_buf)
                    .is_err()
                {
                    return;
                }
                if self.cnt.link.send(&self.cnt.tx_buf).is_err() {
                    return;
                }
                if !self
                    .cnt
                    .wait_msg_processed(self.cnt.ack_check_timeout)
                    .unwrap_or(false)
                    || self.buf.is_finished()
                {
                    break;
                }

                if self.cnt.ack_check_timeout.is_zero() {
                    std::thread::sleep(self.cnt.send_interval);
                }
            }
        }
    }
}

pub struct Controller<L: Link, T: Transducer> {
    link: L,
    geometry: Geometry<T>,
    tx_buf: TxDatagram,
    rx_buf: RxDatagram,
    pub ack_check_timeout: std::time::Duration,
    pub send_interval: std::time::Duration,
    pub force_fan: bool,
    pub reads_fpga_info: bool,
}

impl<L: Link, T: Transducer> Controller<L, T> {
    pub fn open(geometry: Geometry<T>, link: L) -> Result<Controller<L, T>> {
        let mut link = link;
        link.open(&geometry)?;
        let num_devices = geometry.num_devices();
        let tx_buf = TxDatagram::new(geometry.device_map());
        Ok(Controller {
            link,
            geometry,
            tx_buf,
            rx_buf: RxDatagram::new(num_devices),
            ack_check_timeout: std::time::Duration::from_nanos(0),
            send_interval: std::time::Duration::from_micros(
                autd3_core::EC_CYCLE_TIME_BASE_MICRO_SEC as _,
            ),
            force_fan: false,
            reads_fpga_info: false,
        })
    }
}

impl<L: Link, T: Transducer> Controller<L, T> {
    pub fn geometry(&self) -> &Geometry<T> {
        &self.geometry
    }

    pub fn sound_speed(&self) -> f64 {
        self.geometry[0].sound_speed()
    }

    pub fn set_sound_speed(&mut self, value: f64) {
        self.geometry
            .transducers_mut()
            .for_each(|tr| tr.set_sound_speed(value));
    }

    /// Send header and body to the devices
    ///
    /// # Arguments
    ///
    /// * `header` - Header
    /// * `body` - Body
    ///
    pub fn send<'a, 'b, S: Sendable<T>>(
        &'a mut self,
        s: &'b mut S,
    ) -> Sender<'a, 'b, L, T, S, S::H, S::B> {
        Sender::new(self, s)
    }

    /// Return firmware information of the devices
    pub fn firmware_infos(&mut self) -> Result<Vec<FirmwareInfo>> {
        autd3_core::cpu_version(&mut self.tx_buf);
        self.link.send(&self.tx_buf)?;
        self.wait_msg_processed(std::time::Duration::from_millis(100))?;
        let cpu_versions = self
            .rx_buf
            .messages()
            .iter()
            .map(|rx| rx.ack)
            .collect::<Vec<_>>();

        autd3_core::fpga_version(&mut self.tx_buf);
        self.link.send(&self.tx_buf)?;
        self.wait_msg_processed(std::time::Duration::from_millis(100))?;
        let fpga_versions = self
            .rx_buf
            .messages()
            .iter()
            .map(|rx| rx.ack)
            .collect::<Vec<_>>();

        autd3_core::fpga_functions(&mut self.tx_buf);
        self.link.send(&self.tx_buf)?;
        self.wait_msg_processed(std::time::Duration::from_millis(100))?;
        let fpga_functions = self
            .rx_buf
            .messages()
            .iter()
            .map(|rx| rx.ack)
            .collect::<Vec<_>>();

        Ok((0..self.geometry.num_devices())
            .map(|i| FirmwareInfo::new(0, cpu_versions[i], fpga_versions[i], fpga_functions[i]))
            .collect())
    }

    pub fn close(&mut self) -> Result<bool> {
        let mut stop = Amplitudes::none();
        let res = self.send(&mut stop).flush()?;
        let mut clear = Clear::new();
        let res = res & self.send(&mut clear).flush()?;
        self.link.close()?;
        Ok(res)
    }
}

impl<L: Link, T: Transducer> Controller<L, T> {
    pub fn get_id(&self) -> u8 {
        if MSG_ID
            .compare_exchange(
                MSG_END,
                MSG_BEGIN,
                atomic::Ordering::SeqCst,
                atomic::Ordering::SeqCst,
            )
            .is_err()
        {
            MSG_ID.fetch_add(1, atomic::Ordering::SeqCst);
        }
        MSG_ID.load(atomic::Ordering::SeqCst)
    }

    fn wait_msg_processed(&mut self, timeout: std::time::Duration) -> Result<bool> {
        let msg_id = self.tx_buf.header().msg_id;
        let start = std::time::Instant::now();
        while std::time::Instant::now() - start < timeout {
            if self.link.receive(&mut self.rx_buf)? && self.rx_buf.is_msg_processed(msg_id) {
                return Ok(true);
            }
            std::thread::sleep(self.send_interval);
        }
        Ok(false)
    }
}
