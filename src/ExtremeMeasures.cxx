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

#include "Enumerate.hpp"
#include "EmpiricalDistribution.hpp"
#include "ExtremeMeasures.hpp"
#include <numeric>
#include <cmath>

namespace ejd {

//////////////////////////////////////////////////////////////////////////////
//
// Monotonicity Structure
//
//////////////////////////////////////////////////////////////////////////////

// TODO : implement the non-recursive version
static Eigen::MatrixXi construct_monotonestruct(const int n)
{
	if (n == 2)
	{
		Eigen::Matrix2i ms_2d;	
		ms_2d << 1,1,
			  1,-1;
		return ms_2d;
	}
	else
	{
		Eigen::MatrixXi lower_ms = construct_monotonestruct(n-1);

		int num_columns_ms = ::std::pow(2,n-1);

		Eigen::MatrixXi this_ms;
		this_ms.resize(n, num_columns_ms);

		Eigen::RowVectorXi v_ones(num_columns_ms);
		Eigen::RowVectorXi v_ones_lower(lower_ms.cols());
		v_ones_lower.setOnes();
		v_ones << v_ones_lower, v_ones_lower*-1;

		this_ms << lower_ms, lower_ms, v_ones;

		return this_ms;
	}
};

MonotonicityStructure::MonotonicityStructure(const int dim) 
{
	this->extremePts_ = construct_monotonestruct(dim);
}

int MonotonicityStructure::num_extremepts() const {
	return this->extremePts_.cols();
}

//////////////////////////////////////////////////////////////////////////////
//
// Extreme Measures
//
//////////////////////////////////////////////////////////////////////////////

bool LatticePoint::operator==(const LatticePoint &y) const {
	
	if(point_.size() != y.point_.size()) {
		return false;
	}

	for (int i = 0; i < point_.size(); ++i) {
		if (point_[i] != y.point_[i]) {
			return false;
		}
	}

	return true;
}

bool LatticePoint::operator<(const LatticePoint &y) const {

	if (point_.size() != y.point_.size()) {
		return false;
	}

	bool prev_coord_less_than = false;

	for (int i = 0; i < point_.size(); ++i) {
		if (point_[i] == y.point_[i]) {
			continue;
		}
		else if (point_[i] < y.point_[i]) {
			if (prev_coord_less_than) {
				return false;
			} else {
				return true;
			}
		}
		else {
			prev_coord_less_than = true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const LatticePoint& point)
{
	os << '(';
	auto d = point.point_.size();
	for (int i = 0; i < d; ++i) {
		os << point.point_[i] << ',';
	}
	os << ')';
	return os;
}

//////////////////////////////////////////////////////////////////////////////
//
// Extreme Joint Distribution
//
//////////////////////////////////////////////////////////////////////////////

// TODO refactor and split into two sub functions (into more general utilities)
std::vector<std::vector<double>> flip_EmpDistrArray_CDF(
	const std::vector<EmpiricalDistribution> marginal_pdfs,
	const std::vector<int> monotone_struture) 
{
	std::vector<std::vector<double>> marginal_cdf;

	for (int i = 0; i < marginal_pdfs.size(); ++i) 
	{
		marginal_cdf.push_back(marginal_pdfs[i].weights);

		if (monotone_struture[i] == -1) {
			std::reverse(std::begin(marginal_cdf[i]),std::end(marginal_cdf[i]));
		}
	}
	std::for_each(
		marginal_cdf.begin(), marginal_cdf.end(),
		[](auto & z){
			apply_cumsum(&z);
		}
	);
	return marginal_cdf;
}

std::vector<std::vector<double>> flip_supports(
	const std::vector<EmpiricalDistribution> marginal_pdfs,
	const std::vector<int> monotone_structs)
{
	std::vector<std::vector<double>> flipped_support;

	for (int i = 0; i < marginal_pdfs.size(); ++i) 
	{
		flipped_support.push_back(marginal_pdfs[i].support);

		if (monotone_structs[i] == -1) {
			std::reverse(std::begin(flipped_support[i]), std::end(flipped_support[i]));
		}
	}
	return flipped_support;
}

void ensure_right_tail(std::vector<double> * prob_distr_) {

	std::vector<double> & prob_distr = * prob_distr_;

	// note : in essence, machine eps is hard-coded
	auto indices = std::remove_if(
		std::begin(prob_distr), std::end(prob_distr),
		[](double x){
			return( std::abs(x-1) <= 1e-9);
		}
	);
	prob_distr.erase(indices,std::end(prob_distr));
	prob_distr.push_back(1.0);
}

ExtremeMeasure ejd(EmpDistrArray empdistrarrs, std::vector<int> monotone_structs) {

	auto b = ExtremeMeasure();		// so this compiles
	std::vector<LatticePoint> latticePoints;

	// algorithm works on the cdf
	// copy the raw marginals and process such that it is consistent with the monotone structure
	std::vector<std::vector<double>> marginal_cdfs = flip_EmpDistrArray_CDF(empdistrarrs.marginals, monotone_structs);

	std::vector<std::vector<double>> marginal_supports = flip_supports(empdistrarrs.marginals,
		monotone_structs);

	std::vector<int> support_lengths;
	for(auto i : marginal_cdfs) {
		support_lengths.push_back(i.size());
	}

	auto jointcdf = flatten(marginal_cdfs);
	std::sort(jointcdf.begin(), jointcdf.end());
	std::unique(jointcdf.begin(), jointcdf.end());
	ensure_right_tail(&jointcdf);
	auto support_length = jointcdf.size();
	
	// weights of the Extreme Measure can be easily obtained from the combined CDF points
	// from the marginals CDFs
	auto weights = jointcdf;
	std::adjacent_difference(weights.begin(), weights.end(), weights.begin());

	std::vector<LatticePoint> support;

	// find the support of the Extreme Measures
	// we know the length of the support of the Extreme Measure since the marginals have to be satisfied
	for (int i = 0; i < support_length; ++i) 
	{
		std::vector<int> ith_support;	// ensure that it gets reset each iteration

		for (auto && [marginalcdf_index, marginalcdf_value] : enumerate(marginal_cdfs))
		{
			auto temp_jointval = jointcdf[i];

			auto a = std::find_if(
				marginalcdf_value.begin(), marginalcdf_value.end(),
				[&](const double marginalcdf_value_){
					return ( marginalcdf_value_ >= temp_jointval);
				}
			);
			if (a == std::end(marginalcdf_value)) 
			{
				// should be the case where all the probabilities from the ith marginal is allocated to the joint measure
				ith_support.push_back(marginalcdf_value.size());
			} 
			else
			{
				ith_support.push_back(std::distance(marginalcdf_value.begin(),a));
			}
		}
		for(int i = 0; i < ith_support.size(); ++i) {
			ith_support[i] = marginal_supports[i][static_cast<int>(ith_support[i])];
		}
		support.emplace_back(LatticePoint(ith_support));
	}
	return {.support=support, .weights=weights, .monotone_structure=monotone_structs};
}
// namespace ejd	
}
