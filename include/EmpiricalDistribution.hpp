#pragma once

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
