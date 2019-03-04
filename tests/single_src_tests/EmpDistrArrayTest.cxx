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

#include "Enumerate.hpp"
#include "PrettyPrint.hpp"
#include "EmpiricalDistribution.hpp"
#include "ExtremeMeasures.hpp"
// boost
#include "boost/math/distributions.hpp"
#include "Eigen/Core"
// std libs
#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <numeric>

#include "gtest/gtest.h"

namespace bm = boost::math;

constexpr bool TEST_UPPER_BOUND = false;
constexpr bool TEST_EMP_DISTR_ARRAY = false;
constexpr bool TEST_LATTICE_POINT = false;
constexpr bool EJD_SCRATCH = true;

struct EmpDistrArrayTests : public ::testing::Test 
{
    std::vector<int> poisson_params {3,5,7,9};
    std::vector<boost::math::poisson> poiss_vec;

    virtual void SetUp() override {    
        for (const auto & e : poisson_params) {
            poiss_vec.emplace_back(boost::math::poisson(e));
        }
    }
};

TEST_F(EmpDistrArrayTests, upper_bounds) {
    
    EXPECT_EQ(
        ejd::upper_bounds(poiss_vec[1])
    ,17);
}

int main(int argc, char **argv)
{
    /* code */
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

