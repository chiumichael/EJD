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

#include "Correlation.hpp"
#include "EmpiricalDistribution.hpp"
#include "ExtremeMeasures.hpp"
// 3rd party lib
#include "Discreture/Combinations.hpp"
// std lib
#include <cassert>

namespace ejd {

namespace detail  {

double bivariate_expectation(std::vector<LatticePoint> support, std::vector<double> weights)
{
    // note : for now, ensure marginals are unidimensional
    assert(support[0].dimension() == 2);
    double bivarexp = 0.;
    for (int i = 0; i < support.size(); ++i) {
        bivarexp += support[i].product() * weights[i];
    }
    return bivarexp;
}

double correlation(std::vector<LatticePoint> support, std::vector<double> weights, std::vector<double> means, std::vector<double> variances)
{
    assert(support[0].dimension() == 2);

    double bivarexp = bivariate_expectation(support, weights);

    return ( bivarexp - means[0] * means[1] ) / std::sqrt( variances[0] * variances[1] );
}

}   // namespace detail

double correlation(const ExtremeMeasure& em)
{
    return detail::correlation(em.support, em.weights, em.means, em.variances);
}

//////////////////////////////////////////////////////////////////////////////
//
// Extreme Measure
//
//////////////////////////////////////////////////////////////////////////////

// note : does not output the indices of the correlation
std::vector<double> correlations(const ExtremeMeasure & em) 
{
    // get the combinatorial pairwise indices in the desired lexicographical order: i.e, 0 1, 0 2, 0 3, 1 2, ...
    auto pairwise_indices = discreture::lex_combinations(em.dimension(), 2);

    std::vector<double> corr;
    corr.reserve(pairwise_indices.size());

    // NEED marginalization functionality
    for (auto && this_pair : pairwise_indices) {
        // be careful about pushing back / inserting into a reserved vector
    }
    return corr;
}

//////////////////////////////////////////////////////////////////////////////
//
// 2d Poisson Correlation Bounds
//
//////////////////////////////////////////////////////////////////////////////

// TODO finish
std::pair<double,double> poiss_correlation_bounds_2d(const double intensity1, const double intensity2) 
{
    auto poisson_2d_ems = construct_Poisson_ExtremeMeasures({intensity1,intensity2});

    const double max_corr = correlation(poisson_2d_ems[0]);
    const double min_corr = correlation(poisson_2d_ems[1]);

    return std::make_pair(max_corr, min_corr);
}

// namespace ejd
}
