#pragma once
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

#include <utility>
#include <vector>

namespace ejd {

// fwd declarations
struct LatticePoint;
struct ExtremeMeasure;

namespace detail  {

double bivariate_expectation(std::vector<LatticePoint> support, std::vector<double> weights);

double correlation(std::vector<LatticePoint> support, std::vector<double> weights, double mean, double variance);

}   // namespace detail

double correlation(const ExtremeMeasure& em);

//////////////////////////////////////////////////////////////////////////////
//
// Extreme Measure 
//
//////////////////////////////////////////////////////////////////////////////

std::vector<double> correlations(const ExtremeMeasure & em);

//////////////////////////////////////////////////////////////////////////////
//
// 2d Poisson Correlation Bounds
//
//////////////////////////////////////////////////////////////////////////////

// returns the [min, max] admissible correlation bounds for a Poisson process with the specified intensities
std::pair<double,double> poiss_correlation_bounds_2d(const double intensity1, const double intensity2);

// namespace ejd
}
