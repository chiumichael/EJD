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
#include "Correlation.hpp"
#include "PrettyPrint.hpp"

#include "gtest/gtest.h"

using namespace ejd;

TEST(BivariateExpectationsTest, TEST_1) {
    
}

struct PoissonCorrelationTest2d : public ::testing::Test {
    // first construct the 2d ExtremeMeasures
    ExtremeMeasures Poisson_EM = construct_Poisson_ExtremeMeasures({3,5});
};

// TEST_F(PoissonCorrelationTest2d, BivariateExpectation_TEST) {
//     // PrettyPrint(Poisson_EM);         // this works

//     const ExtremeMeasure& pem1 = Poisson_EM[0];
//     const ExtremeMeasure& pem2 = Poisson_EM[1];

//     PrettyPrint(pem1.monotone_structure);

//     std::cout << std::setprecision(10) << "bivariate_expectation 1: " << ejd::detail::bivariate_expectation(pem1.support, pem1.weights) << std::endl;

//     std::cout << "correlation: " << ejd::detail::correlation(pem1) << std::endl;

//     PrettyPrint(pem1.support, true);
//     PrettyPrint(pem1.weights, true);

//     std::cout << std::setprecision(10) << "bivariate_expectation 2: " << ejd::detail::bivariate_expectation(pem2.support, pem2.weights) << std::endl;

//     std::cout << "correlation: " << ejd::detail::correlation(pem2) << std::endl;

//     PrettyPrint(pem2.support, true);
//     PrettyPrint(pem2.weights, true);

//     EXPECT_TRUE(true);
// }

TEST_F(PoissonCorrelationTest2d, BivariateExpectation_TEST) {
    const ExtremeMeasure& pem1 = Poisson_EM[0];
    const ExtremeMeasure& pem2 = Poisson_EM[1];

    double pem1_corr = ejd::correlation(pem1);
    double pem2_corr = ejd::correlation(pem2);

    EXPECT_NEAR(pem1_corr, 0.9767785568527073,1e-3);
    EXPECT_NEAR(pem2_corr, -0.9387482567435699,1e-3);
}

int main(int argc, char **argv)
{
    /* code */
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
