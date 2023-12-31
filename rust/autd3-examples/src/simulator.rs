/*
 * File: simulator.rs
 * Project: src
 * Created Date: 10/10/2022
 * Author: Shun Suzuki
 * -----
 * Last Modified: 05/12/2022
 * Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
 * -----
 * Copyright (c) 2022 Shun Suzuki. All rights reserved.
 *
 */

mod test_runner;
mod tests;

use anyhow::Result;

use autd3::prelude::*;
use autd3_link_simulator::Simulator;

fn main() -> Result<()> {
    let mut geometry = GeometryBuilder::new().legacy_mode().build();
    geometry.add_device(AUTD3::new(Vector3::zeros(), Vector3::zeros()))?;
    geometry.add_device(AUTD3::new(
        Vector3::new(DEVICE_WIDTH, 0.0, 0.0),
        Vector3::zeros(),
    ))?;

    let link = Simulator::new();

    let autd = Controller::open(geometry, link).expect("Failed to open");

    run!(autd);

    Ok(())
}
