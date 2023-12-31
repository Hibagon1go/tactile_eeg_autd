/*
 * File: greedy.rs
 * Project: combinational
 * Created Date: 03/06/2021
 * Author: Shun Suzuki
 * -----
 * Last Modified: 05/12/2022
 * Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
 * -----
 * Copyright (c) 2021 Shun Suzuki. All rights reserved.
 *
 */

use std::f64::consts::PI;

use crate::{constraint::Constraint, macros::propagate, Complex};
use anyhow::Result;
use autd3_core::{
    gain::GainProps,
    geometry::{Geometry, Transducer, Vector3},
};
use autd3_traits::Gain;
use nalgebra::ComplexField;

/// Reference
/// * Shun Suzuki, Masahiro Fujiwara, Yasutoshi Makino, and Hiroyuki Shinoda, “Radiation Pressure Field Reconstruction for Ultrasound Midair Haptics by Greedy Algorithm with Brute-Force Search,” in IEEE Transactions on Haptics, doi: 10.1109/TOH.2021.3076489
#[derive(Gain)]
pub struct Greedy<C: Constraint> {
    props: GainProps,
    foci: Vec<Vector3>,
    amps: Vec<f64>,
    phase_candidates: Vec<Complex>,
    constraint: C,
}

impl<C: Constraint> Greedy<C> {
    pub fn new(foci: Vec<Vector3>, amps: Vec<f64>, constraint: C) -> Self {
        Self::with_param(foci, amps, constraint, 16)
    }

    pub fn with_param(foci: Vec<Vector3>, amps: Vec<f64>, constraint: C, phase_div: usize) -> Self {
        assert!(foci.len() == amps.len());
        let mut phase_candidates = Vec::with_capacity(phase_div);
        for i in 0..phase_div {
            phase_candidates.push(Complex::new(0., 2.0 * PI * i as f64 / phase_div as f64).exp());
        }
        Self {
            props: GainProps::default(),
            foci,
            amps,
            phase_candidates,
            constraint,
        }
    }

    fn transfer_foci<T: Transducer>(
        trans: &T,
        phase: Complex,
        foci: &[Vector3],
        res: &mut [Complex],
    ) {
        for i in 0..foci.len() {
            res[i] = propagate(trans, foci[i]) * phase;
        }
    }

    fn calc<T: Transducer>(&mut self, geometry: &Geometry<T>) -> Result<()> {
        let m = self.foci.len();

        let mut tmp = Vec::with_capacity(self.phase_candidates.len());
        tmp.resize(self.phase_candidates.len(), vec![Complex::new(0., 0.); m]);

        let mut cache = Vec::with_capacity(m);
        cache.resize(m, Complex::new(0., 0.));

        geometry.transducers().for_each(|trans| {
            let mut min_idx = 0;
            let mut min_v = std::f64::INFINITY;
            for (idx, &phase) in self.phase_candidates.iter().enumerate() {
                Self::transfer_foci(trans, phase, &self.foci, &mut tmp[idx]);
                let mut v = 0.0;
                for (j, c) in cache.iter().enumerate() {
                    v += (self.amps[j] - (tmp[idx][j] + c).abs()).abs();
                }

                if v < min_v {
                    min_v = v;
                    min_idx = idx;
                }
            }

            for (j, c) in cache.iter_mut().enumerate() {
                *c += tmp[min_idx][j];
            }

            let phase = self.phase_candidates[min_idx].argument() + PI;
            let amp = self.constraint.convert(1.0, 1.0);
            self.props.drives[trans.id()].amp = amp;
            self.props.drives[trans.id()].phase = phase;
        });
        Ok(())
    }
}
