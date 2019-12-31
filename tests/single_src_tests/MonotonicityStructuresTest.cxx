/*
    This file is part of EJD.

    Copyright © 2019
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
// std lib
#include <iostream>

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

TEST(MonotonicityStructure, 3d) {
    auto b = ejd::constructMonotoneStructure(3);

    std::cout << b << std::endl;

    EXPECT_TRUE(true);
}

TEST(MonotonicityStructure, 4d) {
    auto b = ejd::constructMonotoneStructure(4);

    std::cout << b << std::endl;

    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
