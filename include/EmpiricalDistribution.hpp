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
//std libs
#include <vector>
// boost libs
#include "boost/math/distributions.hpp"		// includes all distributions

namespace ejd {

//////////////////////////////////////////////////////////////////////////////
//
// General Helper Functions 
//
//////////////////////////////////////////////////////////////////////////////

void edit_sum_1(std::vector<double> v);

//////////////////////////////////////////////////////////////////////////////
//
// Probabilistic Helper Functions 
//
//////////////////////////////////////////////////////////////////////////////

template<typename T>
double discrete_nth_moment(std::vector<T> weights, std::vector<T> support, const int N)
{
	double moment = 0;

	for ( auto [w,s] : zip(weights,support) ) {
		moment += w * std::pow(s,N);
	}
	return moment;
}

template <typename T>	// restrict T to numeric types in the future w/ concepts
double discrete_empirical_mean(std::vector<T> weights, std::vector<T> support) {
	// note : type of weights and support should not _need_ to be equal
	return discrete_nth_moment<T>(weights,support,1);
}

template <typename T>
double discrete_empirical_variance(const std::vector<T> weights, const std::vector<T> support) {
	double mean = discrete_empirical_mean(weights,support);
	double second_moment = discrete_nth_moment<T>(weights,support,2);
	return second_moment - std::pow(mean,2);
}

//////////////////////////////////////////////////////////////////////////////
//
// Empirical Distribution 
//
//////////////////////////////////////////////////////////////////////////////

struct EmpiricalDistribution {
	std::vector<double> weights;
	std::vector<double> support;
	// methods
	double mean() const;
	double variance() const;
	double total_prob() const;
	double entropy() const;
};

// TO-DO : force variable precision
template <typename Distribution>
auto construct_discrete_EmpDistr(Distribution distr, int support_end) 
	-> EmpiricalDistribution
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
			return boost::math::pdf(distr,x);
		}
	);		
	return EmpiricalDistribution {.weights = weights, .support = support};
}

//////////////////////////////////////////////////////////////////////////////
//
// Empirical Distribution Array 
//
//////////////////////////////////////////////////////////////////////////////

// TO-DO : does not take into account tolerance!!
// Empirical Distribution Array
struct EmpDistrArray
{
	// data
	std::vector<EmpiricalDistribution> marginals;
	// member functions
	std::vector<double> means() const;
	std::vector<double> variances() const;
	int dimensions() const;
};

// namespace ejd
}
