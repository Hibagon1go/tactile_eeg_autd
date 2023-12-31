/*
 * File: macros.rs
 * Project: src
 * Created Date: 28/05/2021
 * Author: Shun Suzuki
 * -----
 * Last Modified: 05/12/2022
 * Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
 * -----
 * Copyright (c) 2021 Shun Suzuki. All rights reserved.
 *
 */

use crate::{Complex, MatrixXc};
use autd3_core::{
    acoustics::directivity_t4010a1 as directivity,
    geometry::{Geometry, Transducer, Vector3},
};
#[allow(unused)]
use nalgebra::ComplexField;

pub fn propagate<T: Transducer>(source: &T, target: Vector3) -> Complex {
    let diff = target - source.position();
    let dist = diff.norm();
    let theta = source.z_direction().angle(&diff);

    let d = directivity(theta);
    let r = d * (-dist * source.attenuation()).exp() / dist;
    let phi = -source.wavenumber() * dist;
    r * Complex::new(0., phi).exp()
}

pub fn generate_propagation_matrix<T: Transducer>(
    geometry: &Geometry<T>,
    foci: &[Vector3],
) -> MatrixXc {
    let m = foci.len();
    let num_trans = geometry.num_transducers();

    MatrixXc::from_iterator(
        m,
        num_trans,
        geometry
            .transducers()
            .flat_map(|trans| foci.iter().map(move |&fp| propagate(trans, fp))),
    )
}
