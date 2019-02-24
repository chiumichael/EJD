
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

EmpDistrArray construct_EmpDistrArray(std::vector<bm::poisson> poisson_distrs) 
{
	std::vector<EmpiricalDistribution> emp_distr_data;

	// calculate the max length of uspport 
	int max_upper_bound = 0;
	for (int i = 0; i < poisson_distrs.size(); ++i) {
		int this_ub = upper_bounds(poisson_distrs[i]);
		max_upper_bound = max_upper_bound < this_ub ? this_ub : max_upper_bound;
	}

	for (int i = 0; i < poisson_distrs.size(); ++i) {
		emp_distr_data.emplace_back(
			construct_discrete_EmpDistr(poisson_distrs[i], max_upper_bound)
		);
	}

	return EmpDistrArray(emp_distr_data);
}

// namespace ejd
}
