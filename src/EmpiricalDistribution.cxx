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

#include "EmpiricalDistribution.hpp"
// std libs
#include <algorithm>
#include <cmath>

namespace ejd {

//////////////////////////////////////////////////////////////////////////////
//
// General Helper Functions 
//
//////////////////////////////////////////////////////////////////////////////

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

bool valid_emp_distr(const std::vector<double>& p, double errtol) {
	
	double sum;
	std::for_each(p.begin(), p.end(),
		[&sum](double p_){ sum += p_; }
	);
	if (sum - 1 < errtol) {
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////
//
// Empirical Distribution 
//
//////////////////////////////////////////////////////////////////////////////

// TODO : finish me!
bool EmpiricalDistribution::operator==(const EmpiricalDistribution &rhs) const {
	return std::equal(this->weights.begin(), this->weights.end(),rhs.weights.begin(), rhs.weights.end()) 
		&& std::equal(this->support.begin(), this->support.end(),rhs.support.begin(), rhs.support.end());
}

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

bool EmpDistrArray::operator==(const EmpDistrArray& rhs) const {
	return std::equal(this->marginals.begin(), this->marginals.end(),rhs.marginals.begin(), rhs.marginals.end());
}

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
		[] (const EmpiricalDistribution & empdistr) {
			return empdistr.variance();
		}
	);

	return variances;
}

int EmpDistrArray::dimensions() const {
	return marginals.size();
}

EmpDistrArray construct_EmpDistrArray(const std::vector<bm::poisson>& poisson_distrs) 
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

EmpDistrArray construct_Poisson_EmpDistrArray(const std::vector<double>& intensities)
{
	std::vector<boost::math::poisson> poiss_distrs;
	poiss_distrs.reserve(intensities.size());

	for (const auto & e : intensities) {
		poiss_distrs.emplace_back(boost::math::poisson(e));
	}
	return construct_EmpDistrArray(poiss_distrs);
}
// namespace ejd
}
