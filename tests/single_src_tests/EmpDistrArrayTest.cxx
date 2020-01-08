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

#include "Utils/Enumerate.hpp"
#include "Utils/PrettyPrint.hpp"
#include "EmpiricalDistribution.hpp"
#include "ExtremeMeasures.hpp"
// boost
#include "boost/math/distributions.hpp"
namespace bm = boost::math;
// #include "Eigen/Core"

// std libs
#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <numeric>

#include "gtest/gtest.h"

using namespace ejd;

//////////////////////////////////////////////////////////////////////////////
//
// Empirical Distribution Tests
//
//////////////////////////////////////////////////////////////////////////////

struct EmpDistrTests : public ::testing::Test {
    std::vector<int> poisson_params {3,5,7,9};
    
};

//////////////////////////////////////////////////////////////////////////////
//
// Empirical Distribution Array Tests
//
//////////////////////////////////////////////////////////////////////////////

struct EmpDistrArrayTests : public ::testing::Test 
{
    std::vector<double> poisson_params {3,5,7,9};
    std::vector<boost::math::poisson> poisson_distrs;
    EmpDistrArray empdistrarray_poiss;

    EmpDistrArrayTests() {    
        for (const auto & e : poisson_params) {
            poisson_distrs.emplace_back(boost::math::poisson(e));
        }
        empdistrarray_poiss = construct_EmpDistrArray(poisson_distrs);
    }
};

TEST_F(EmpDistrArrayTests, upper_bounds) {
    EXPECT_EQ(
        ejd::upper_bounds(poisson_distrs[1])
    ,17);
}

TEST_F(EmpDistrArrayTests, CONTSTRUCTION_TEST_1) {
    EXPECT_EQ(
        empdistrarray_poiss.dimensions() 
        ,4
    );
}

TEST_F(EmpDistrArrayTests, construct_Poisson_EmpDistrArray) {
    auto a = ejd::construct_Poisson_EmpDistrArray(poisson_params);
}

int main(int argc, char **argv)
{
    /* code */
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
