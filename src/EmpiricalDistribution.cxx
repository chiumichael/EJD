
#include "EmpiricalDistribution.hpp"
// std libs
#include <algorithm>
#include <cmath>

namespace ejd {

void edit_sum_1(std::vector<double> * v_ptr) {

	std::vector<double> & v = * v_ptr;

	int v_size = v.size();

	double sum_n_minus_1 = std::accumulate(
		v.begin(),
		v.begin() + v_size - 1,
		0.0
	);

	v[v_size-1] = 1 - sum_n_minus_1;
}

//////////////////////////////////////////////////////////////////////////////
//
// Empirical Distribution 
//
//////////////////////////////////////////////////////////////////////////////

double EmpiricalDistribution::mean() const {
	return discrete_empirical_mean(weights,support);
}

double EmpiricalDistribution::variance() const {
	return discrete_empirical_variance(weights,support);
}

double EmpiricalDistribution::total_prob() const {
	return std::accumulate(
		weights.begin(),
		weights.end(),
		0.0
	);
}

double EmpiricalDistribution::entropy() const {
	double ent = 0;
	for (const auto & w : weights) {
		ent += w * std::log(w);
	}
	return -ent;
}

//////////////////////////////////////////////////////////////////////////////
//
// Empirical Distribution Array 
//
//////////////////////////////////////////////////////////////////////////////

std::vector<double> EmpDistrArray::means() const {
	std::vector<double> means(marginals.size(),0.);

	std::transform(
		marginals.begin(),
		marginals.end(),
		means.begin(),
		[](const EmpiricalDistribution & empdistr) {
			return empdistr.mean();
		}
	);

	return means;
}

std::vector<double> EmpDistrArray::variances() const {
	std::vector<double> variances(marginals.size(),0.);

	std::transform(
		marginals.begin(),
		marginals.end(),
		variances.begin(),
		[](const EmpiricalDistribution & empdistr) {
			return empdistr.variance();
		}
	);

	return variances;
}

// namespace ejd
}
