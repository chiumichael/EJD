/*
    This file is part of EJD.

    Copyright © 2020
              Michael Chiu <chiu@cs.toronto.edu>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "ExtremeMeasures.hpp"
// 3rd party
#include "gtest/gtest.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <fmt/format.h>
#include <blaze/math/Column.h>
namespace b = blaze;
// std lib
#include <chrono>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>
#include <sstream>

TEST(MonotonicityStructure, SIZE_TEST_n_2) {
    auto size = ejd::monotoneStructSize(2);
    
    EXPECT_EQ(
        size.first,
        2
    );

    EXPECT_EQ(
        size.second,
        2
    );
}

TEST(MonotonicityStructure, SIZE_TEST_n_3) {
    auto size = ejd::monotoneStructSize(3);

    EXPECT_EQ(
        size.first,
        3
    );

    EXPECT_EQ(
        size.second,
        4
    );
}

TEST(MonotonicityStructure, SIZE_TEST_n_4) {
    auto size = ejd::monotoneStructSize(4);

    EXPECT_EQ(
        size.first,
        4
    );

    EXPECT_EQ(
        size.second,
        8
    );
}

TEST(MonotonicityStructure, SIZE_TEST_n_5) {
    auto size = ejd::monotoneStructSize(5);

    EXPECT_EQ(
        size.first,
        5
    );

    EXPECT_EQ(
        size.second,
        16
    );
}

struct MonotoneStructTests : public ::testing::Test {
    fs::path dataDir = fs::current_path() / "tests/data";

    void run(const int dim) {
        auto b = ejd::constructMonotoneStructure(dim);

        fs::path currPath = fs::current_path();

        std::stringstream ss;
        ss << "ms" << dim << ".json";

        fs::path datapath = currPath / "tests/data" / ss.str();

        if ( !fs::exists(datapath) ) {
            ASSERT_FALSE(true) << "file does not exist";
        }

        std::ifstream f(datapath.c_str());

        json julia_data;
        f >> julia_data;

        auto [m, n] = ejd::monotoneStructSize(dim);

        for (size_t j = 0; j < n; ++j) {
            auto col = b::column(b,j);
            auto o = julia_data.at(j).get<std::vector<int>>();

            for (size_t i = 0; i < m; ++i) {
                if (col[i] != o[i]) {
                    ASSERT_FALSE(true);
                }
            }
        }
        EXPECT_TRUE(true);
    }
};

TEST_F(MonotoneStructTests, 3d) {
    auto start = std::chrono::system_clock::now();
    run(3);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secs = end-start;
    fmt::print("Elapsed time: {}\n", elapsed_secs.count());
}

TEST_F(MonotoneStructTests, 4d) {
    auto start = std::chrono::system_clock::now();
    run(4);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secs = end-start;
    fmt::print("Elapsed time: {}\n", elapsed_secs.count());
}

TEST_F(MonotoneStructTests, 5d) {
    auto start = std::chrono::system_clock::now();
    run(5);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secs = end-start;
    fmt::print("Elapsed time: {}\n", elapsed_secs.count());
}

TEST_F(MonotoneStructTests, 6d) {
    auto start = std::chrono::system_clock::now();
    run(6);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secs = end-start;
    fmt::print("Elapsed time: {}\n", elapsed_secs.count());
}

TEST_F(MonotoneStructTests, 7d) {
    auto start = std::chrono::system_clock::now();
    run(7);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secs = end-start;
    fmt::print("Elapsed time: {}\n", elapsed_secs.count());
}

TEST_F(MonotoneStructTests, 8d) {
    auto start = std::chrono::system_clock::now();
    run(8);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secs = end-start;
    fmt::print("Elapsed time: {}\n", elapsed_secs.count());
}

TEST_F(MonotoneStructTests, 9d) {
    auto start = std::chrono::system_clock::now();
    run(9);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secs = end-start;
    fmt::print("Elapsed time: {}\n", elapsed_secs.count());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
