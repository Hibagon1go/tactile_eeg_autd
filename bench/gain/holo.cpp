// File: holo.cpp
// Project: gain
// Created Date: 12/12/2022
// Author: Shun Suzuki
// -----
// Last Modified: 12/12/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#include <benchmark/benchmark.h>

#include <autd3/gain/holo.hpp>
#include <random>

#include "../utils.hpp"

constexpr size_t FOCI_SIZE = 10;

static std::vector<autd3::Vector3> gen_foci(const autd3::Vector3& center, const size_t n, const double rx, const double ry, const double rz,
                                            const int32_t seed = 0) {
  std::vector<autd3::Vector3> foci;
  foci.reserve(n);

  std::mt19937 mt(seed);
  std::normal_distribution norm_x(-rx, rx);
  std::normal_distribution norm_y(-ry, ry);
  std::normal_distribution norm_z(-rz, rz);

  for (size_t i = 0; i < n; i++) foci.emplace_back(center + autd3::Vector3(norm_x(mt), norm_y(mt), norm_z(mt)));

  return foci;
}

static void bm_gain_holo_sdp(benchmark::State& state) {
  autd3::Controller autd;
  setup_autd(autd, state.range(0), state.range(1));

  const autd3::Vector3 center = autd.geometry().center() + autd3::Vector3(0.0, 0.0, 300.0);

  const auto foci = gen_foci(center, FOCI_SIZE, 100, 100, 100);

  const auto backend = autd3::gain::holo::EigenBackend::create();
  autd3::gain::holo::SDP g(backend);

  for (auto& focus : foci) g.add_focus(focus, 1.0);

  for (auto _ : state) {
    g.rebuild(autd.geometry());
  }
}

static void bm_gain_holo_evd(benchmark::State& state) {
  autd3::Controller autd;
  setup_autd(autd, state.range(0), state.range(1));

  const autd3::Vector3 center = autd.geometry().center() + autd3::Vector3(0.0, 0.0, 300.0);

  const auto foci = gen_foci(center, FOCI_SIZE, 100, 100, 100);

  const auto backend = autd3::gain::holo::EigenBackend::create();
  autd3::gain::holo::EVD g(backend);

  for (auto& focus : foci) g.add_focus(focus, 1.0);

  for (auto _ : state) {
    g.rebuild(autd.geometry());
  }
}

static void bm_gain_holo_lss(benchmark::State& state) {
  autd3::Controller autd;
  setup_autd(autd, state.range(0), state.range(1));

  const autd3::Vector3 center = autd.geometry().center() + autd3::Vector3(0.0, 0.0, 300.0);

  const auto foci = gen_foci(center, FOCI_SIZE, 100, 100, 100);

  const auto backend = autd3::gain::holo::EigenBackend::create();
  autd3::gain::holo::LSS g(backend);

  for (auto& focus : foci) g.add_focus(focus, 1.0);

  for (auto _ : state) {
    g.rebuild(autd.geometry());
  }
}

static void bm_gain_holo_gs(benchmark::State& state) {
  autd3::Controller autd;
  setup_autd(autd, state.range(0), state.range(1));

  const autd3::Vector3 center = autd.geometry().center() + autd3::Vector3(0.0, 0.0, 300.0);

  const auto foci = gen_foci(center, FOCI_SIZE, 100, 100, 100);

  const auto backend = autd3::gain::holo::EigenBackend::create();
  autd3::gain::holo::GS g(backend);

  for (auto& focus : foci) g.add_focus(focus, 1.0);

  for (auto _ : state) {
    g.rebuild(autd.geometry());
  }
}

static void bm_gain_holo_gspat(benchmark::State& state) {
  autd3::Controller autd;
  setup_autd(autd, state.range(0), state.range(1));

  const autd3::Vector3 center = autd.geometry().center() + autd3::Vector3(0.0, 0.0, 300.0);

  const auto foci = gen_foci(center, FOCI_SIZE, 100, 100, 100);

  const auto backend = autd3::gain::holo::EigenBackend::create();
  autd3::gain::holo::GSPAT g(backend);

  for (auto& focus : foci) g.add_focus(focus, 1.0);

  for (auto _ : state) {
    g.rebuild(autd.geometry());
  }
}

static void bm_gain_holo_lm(benchmark::State& state) {
  autd3::Controller autd;
  setup_autd(autd, state.range(0), state.range(1));

  const autd3::Vector3 center = autd.geometry().center() + autd3::Vector3(0.0, 0.0, 300.0);

  const auto foci = gen_foci(center, FOCI_SIZE, 100, 100, 100);

  const auto backend = autd3::gain::holo::EigenBackend::create();
  autd3::gain::holo::LM g(backend);

  for (auto& focus : foci) g.add_focus(focus, 1.0);

  for (auto _ : state) {
    g.rebuild(autd.geometry());
  }
}

static void bm_gain_holo_greedy(benchmark::State& state) {
  autd3::Controller autd;
  setup_autd(autd, state.range(0), state.range(1));

  const autd3::Vector3 center = autd.geometry().center() + autd3::Vector3(0.0, 0.0, 300.0);

  const auto foci = gen_foci(center, FOCI_SIZE, 100, 100, 100);

  const auto backend = autd3::gain::holo::EigenBackend::create();
  autd3::gain::holo::Greedy g(backend);

  for (auto& focus : foci) g.add_focus(focus, 1.0);

  for (auto _ : state) {
    g.rebuild(autd.geometry());
  }
}

BENCHMARK(bm_gain_holo_sdp)->Args({1, 1})->Args({2, 2})->Args({3, 3})->Args({4, 4});
BENCHMARK(bm_gain_holo_evd)->Args({1, 1})->Args({2, 2})->Args({3, 3})->Args({4, 4});
BENCHMARK(bm_gain_holo_lss)->Args({1, 1})->Args({2, 2})->Args({3, 3})->Args({4, 4});
BENCHMARK(bm_gain_holo_gs)->Args({1, 1})->Args({2, 2})->Args({3, 3})->Args({4, 4});
BENCHMARK(bm_gain_holo_gspat)->Args({1, 1})->Args({2, 2})->Args({3, 3})->Args({4, 4});
BENCHMARK(bm_gain_holo_lm)->Args({1, 1})->Args({2, 2})->Args({3, 3})->Args({4, 4});
BENCHMARK(bm_gain_holo_greedy)->Args({1, 1})->Args({2, 2})->Args({3, 3})->Args({4, 4});
