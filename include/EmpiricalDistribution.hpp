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

#include "Zip.hpp"
#include "boost/math/distributions.hpp"		// includes all distributions
#include <array>
#include <cmath>
#include <vector>

namespace bm = boost::math;

namespace ejd {

//////////////////////////////////////////////////////////////////////////////
//
// General Helper Functions 
//
//////////////////////////////////////////////////////////////////////////////

void edit_sum_1(std::vector<double> * v_ptr);

// checks that empirical distribution sums to 1
bool valid_emp_distr(const std::vector<double>& p, double errtol=1e-7);

//////////////////////////////////////////////////////////////////////////////
//
// Probabilistic Helper Functions 
//
//////////////////////////////////////////////////////////////////////////////

// uni dimensional
template<typename T>
double discrete_nth_moment(const std::vector<T>& weights, const std::vector<T>& support, const int& N)
{
	double moment = 0;

	for ( auto [w,s] : zip(weights,support) ) {
		moment += w * std::pow(s,N);
	}
	return moment;
}

template <typename T>	// restrict T to numeric types in the future w/ concepts
double discrete_empirical_mean(const std::vector<T>& weights, const std::vector<T>& support) {
	// note : type of weights and support should not _need_ to be equal
	return discrete_nth_moment<T>(weights,support,1);
}

template <typename T>
double discrete_empirical_variance(const std::vector<T>& weights, const std::vector<T>& support) {
	double mean = discrete_empirical_mean(weights,support);
	double second_moment = discrete_nth_moment<T>(weights,support,2);
	return second_moment - std::pow(mean,2);
}

template <typename Distribution>
int upper_bounds(const Distribution& d, double errtol=1e-5) {
	std::array<double, 1000> x;
	for (int i = 0; i < 1000; ++i) {
		x[i] = bm::cdf(bm::complement(d, i));
		if (x[i] <= errtol) {
			return i;
		}
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
//
// Empirical Distribution 
//
//////////////////////////////////////////////////////////////////////////////

struct EmpiricalDistribution {
	std::vector<double> weights;
	std::vector<double> support;
	// operators
	bool operator==(const EmpiricalDistribution& rhs) const;
	// methods
	double mean() const;
	double variance() const;
	double total_prob() const;
	double entropy() const;
};

// TODO : force variable precision
template <typename Distribution>
auto construct_discrete_EmpDistr(const Distribution& distr, const int& support_end, bool edit_tail=true) -> EmpiricalDistribution
{
	// create support
	std::vector<double> support(support_end);
	std::iota(support.begin(), support.end(),0);
	// fill in weights
	std::vector<double> weights(support.size());
	std::transform(
		support.begin(),
		support.end(),
		weights.begin(),
		[&distr] (int x) {
			return bm::pdf(distr,x);
		}
	);
	if (edit_tail) {
		edit_sum_1(&weights);
	}
	return EmpiricalDistribution {.weights = weights, .support = support};
}

//////////////////////////////////////////////////////////////////////////////
//
// Empirical Distribution Array 
//
//////////////////////////////////////////////////////////////////////////////

// TODO : does not take into account tolerance!
struct EmpDistrArray
{
	EmpDistrArray() = default;
	EmpDistrArray(const std::vector<EmpiricalDistribution> marginals)
		: marginals(std::move(marginals))
	{}

	// data
	std::vector<EmpiricalDistribution> marginals;
	// operators
	bool operator==(const EmpDistrArray& rhs) const;
	// member functions
	std::vector<double> means() const;
	std::vector<double> variances() const;
	int dimensions() const;
};

// TODO : enable each underlying marginal distribution to have its own max_upperbound, ie, marginals with un-normalized supports
EmpDistrArray construct_EmpDistrArray(const std::vector<bm::poisson>& poisson_distrs);

// convenience function since Poisson distribution used widely
EmpDistrArray construct_Poisson_EmpDistrArray(const std::vector<double>& intensities);

// namespace ejd
}
