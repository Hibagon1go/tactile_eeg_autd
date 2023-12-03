// File: runner.hpp
// Project: examples
// Created Date: 16/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 29/11/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>

// #include "autd3/liblsl/lsl_cpp.h"
#include "tests/advanced.hpp"
#include "tests/bessel.hpp"
#include "tests/flag.hpp"
#include "tests/focus.hpp"
#include "tests/gain_stm.hpp"
#include "tests/group.hpp"
#include "tests/plane.hpp"
#include "tests/soft_stm.hpp"
#ifdef BUILD_GAIN_HOLO
#include "tests/holo.hpp"
#endif
#ifdef BUILD_MODULATION_AUDIO_FILE
#endif
#include "tests/focus_stm.hpp"
#include "tests/am_single.hpp"
#include "tests/lmc.hpp"

#include <chrono>
#include <thread>
#include <iostream>
#include <random>

using std::this_thread::sleep_for;

inline int run(autd3::Controller &autd)
{
    std::vector<std::pair<F, std::string>> tests = {
        std::pair(F{am_single_100}, "AM 100"),
        std::pair(F{am_single_200}, "AM 200"),
        std::pair(F{lm_circle_7_5_5}, "LM 7.5 5 5"),
        std::pair(F{lm_circle_15_5}, "LM 15 5"),
        std::pair(F{lm_circle_7_5_30}, "LM 7.5 5 30"),
        std::pair(F{lm_circle_15_30}, "LM 15 30"),
    };
    using F = std::function<void(autd3::Controller &)>;

    autd.set_sound_speed(340.0e3);
    autd << autd3::clear << autd3::synchronize;

    while (true)
    {
        size_t i = 0;
        for (size_t i = 0; i < tests.size(); i++)
            std::cout << "[" << i << "]: " << tests[i].second << std::endl;
        std::cout << "[Others]: finish." << std::endl;

        std::cout << "Choose number: ";
        std::string in;
        size_t idx;
        getline(std::cin, in);
        std::stringstream s(in);
        if (const auto empty = in == "\n"; !(s >> idx) || idx >= tests.size() || empty)
            break;

        tests[idx].first(autd);
        sleep_for(std::chrono::milliseconds(500));
        autd << autd3::stop;
    }
    autd.close();

    return 0;
}
