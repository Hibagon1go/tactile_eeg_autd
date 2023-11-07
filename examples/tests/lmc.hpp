// File: focus.hpp
// Project: tests
// Created Date: 11/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 17/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#include <algorithm>
#include <autd3.hpp>
#include <vector>

inline void lm_circle_7_5_5(autd3::Controller &autd)
{
    auto config = autd3::SilencerConfig::none();

    autd3::modulation::Static m;

    autd3::FocusSTM stm(autd.get_sound_speed());

    const autd3::Vector3 center = autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH - 10.0, -10.0, 240.0);
    const int radius = 7.5;
    constexpr size_t points_num = radius * 12;
    std::vector<size_t> points(points_num);
    std::iota(points.begin(), points.end(), 0);
    std::transform(points.begin(), points.end(), std::back_inserter(stm), [&](const size_t i)
                   {
    const auto theta = 2.0 * autd3::pi * static_cast<double>(i) / static_cast<double>(points_num);
    return autd3::FocusSTM::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0)); });

    stm.set_frequency(5);

    autd << config << m, stm;
}

inline void lm_circle_15_5(autd3::Controller& autd)
{
    auto config = autd3::SilencerConfig::none();

    autd3::modulation::Static m;

    autd3::FocusSTM stm(autd.get_sound_speed());

    const autd3::Vector3 center = autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH - 10.0, -10.0, 240.0);
    const int radius = 15;
    constexpr size_t points_num = radius * 12;
    std::vector<size_t> points(points_num);
    std::iota(points.begin(), points.end(), 0);
    std::transform(points.begin(), points.end(), std::back_inserter(stm), [&](const size_t i)
        {
            const auto theta = 2.0 * autd3::pi * static_cast<double>(i) / static_cast<double>(points_num);
    return autd3::FocusSTM::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0)); });

    stm.set_frequency(5);

    autd << config << m, stm;
}

inline void lm_circle_7_5_30(autd3::Controller& autd)
{
    auto config = autd3::SilencerConfig::none();

    autd3::modulation::Static m;

    autd3::FocusSTM stm(autd.get_sound_speed());

    const autd3::Vector3 center = autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH - 10.0, -10.0, 240.0);
    const int radius = 7.5;
    constexpr size_t points_num = radius * 12;
    std::vector<size_t> points(points_num);
    std::iota(points.begin(), points.end(), 0);
    std::transform(points.begin(), points.end(), std::back_inserter(stm), [&](const size_t i)
        {
            const auto theta = 2.0 * autd3::pi * static_cast<double>(i) / static_cast<double>(points_num);
    return autd3::FocusSTM::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0)); });

    stm.set_frequency(30);

    autd << config << m, stm;
}

inline void lm_circle_15_30(autd3::Controller& autd)
{
    auto config = autd3::SilencerConfig::none();

    autd3::modulation::Static m;

    autd3::FocusSTM stm(autd.get_sound_speed());

    const autd3::Vector3 center = autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH - 10.0, -10.0, 240.0);
    const int radius = 15;
    constexpr size_t points_num = radius * 12;
    std::vector<size_t> points(points_num);
    std::iota(points.begin(), points.end(), 0);
    std::transform(points.begin(), points.end(), std::back_inserter(stm), [&](const size_t i)
        {
            const auto theta = 2.0 * autd3::pi * static_cast<double>(i) / static_cast<double>(points_num);
    return autd3::FocusSTM::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0)); });

    stm.set_frequency(30);

    autd << config << m, stm;
}