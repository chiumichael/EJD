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
#include "EmpiricalDistribution.hpp"
#include "PrettyPrint.hpp"
// boost
#include "boost/math/distributions.hpp"
// std libs
#include <iostream>
#include <vector>
#include <algorithm>

namespace bm = boost::math;

constexpr bool TEST_UPPER_BOUND = false;
constexpr bool TEST_EMP_DISTR_ARRAY = true;

int main( int argc, char const *argv[] )  {

std::vector<int> poisson_params {3,5,7,9};

boost::math::poisson poiss(3);

std::vector<boost::math::poisson> poiss_vec;

for (const auto & e : poisson_params) {
    poiss_vec.emplace_back(boost::math::poisson(e));
}

// TO-DO add tests for other distributions
if constexpr (TEST_UPPER_BOUND) 
{
    std::cout << ejd::upper_bounds(poiss_vec[1]) << '\n'; 
}

if constexpr (TEST_EMP_DISTR_ARRAY) {
    auto empdistrarray = ejd::construct_EmpDistrArray(poiss_vec);

    auto a = empdistrarray.means();
    PrettyPrint(a);

    // check valid prob distr
    auto d = ejd::valid_emp_distr(empdistrarray.marginals[0].weights);

    std::cout << "is true" << '\n';
    std::cout << d << '\n';

    // check that it sums to 1 - ie tails are edited
    auto sum = std::accumulate(std::begin(empdistrarray.marginals[0].weights), std::end(empdistrarray.marginals[0].weights),0.0);

    std::cout << "sum is : " << sum << "\n";
}

return 0;
}
