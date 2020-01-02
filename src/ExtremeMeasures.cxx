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

#include "AnsiColor.hpp"
#include "Utils/Enumerate.hpp"
#include "EmpiricalDistribution.hpp"
#include "ExtremeMeasures.hpp"
#include "Utils/PrettyPrint.hpp"
// 3rd party libs
#include <blaze/math/Submatrix.h>
// std libs
#include <cmath>

namespace ejd {

using SubmatrixType = blaze::Submatrix<blaze::DynamicMatrix<int, false>, blaze::unaligned, false, true>;

//////////////////////////////////////////////////////////////////////////////
//
// Monotonicity Structure
//
//////////////////////////////////////////////////////////////////////////////

std::pair<int,int> monotoneStructSize(const int n) {
	return {n,std::pow(2,n-1)};
}

static void fillMonotoneStructure(SubmatrixType& ms) {
	if (ms.rows() == 2) {
		// columns also = 2
		ms(0,0) = 1;
		ms(0,1) = 1;
		ms(1,0) = 1;
		ms(1,1) = -1;
	} else {
		int nRows = ms.rows();
		int nCols = ms.columns();

		// note: maybe these should be better named
		int lMatrixStart = 0;
		int rMatrixStart = nCols / 2;	// always even

		int rowStride = nRows - 1;
		int colStride = nCols / 2;

		auto left_sm = b::submatrix(
			ms, 
			lMatrixStart, lMatrixStart,
			rowStride, colStride
		);

		fillMonotoneStructure(left_sm);

		auto right_sm = b::submatrix(
			ms,
			0, rMatrixStart,
			rowStride, colStride
		);

		right_sm = left_sm;

		// bottom rows;
		auto leftBottomRow = submatrix(
			ms,
			nRows-1,0,
			1, colStride
		);
		auto rightBottomRow = submatrix(
			ms,
			nRows-1, colStride,
			1, colStride
		);

		leftBottomRow = 1;
		rightBottomRow = -1;
	}
}

b::DynamicMatrix<int> constructMonotoneStructure(const int n) {
	std::pair<int, int> ms_size = monotoneStructSize(n);
	b::DynamicMatrix<int> ms(ms_size.first, ms_size.second);
	auto submatrixView = b::submatrix(
		ms,
		0,0,
		ms.rows(), ms.columns()
	);
	fillMonotoneStructure(submatrixView);
	return ms;
}

MonotonicityStructure::MonotonicityStructure(const int dim) {
	this->extremePoints = constructMonotoneStructure(dim);
}

int MonotonicityStructure::num_extremepts() const {
	return this->extremePoints.columns();
}

std::pair<int, int> MonotonicityStructure::size() const {
	return std::pair(extremePoints.rows(),extremePoints.columns());	
}

// b::DynamicVector<int> MonotonicityStructure::operator[](int this_col) const
std::vector<int> MonotonicityStructure::operator[](int this_col) const
{
	// TODO : return native vector eventuallya
	auto bcol = b::column(this->extremePoints,this_col);
	std::vector<int> retCol(bcol.size());

	for (size_t i = 0; i < bcol.size(); ++i) {
		retCol[i] = bcol[i];
	}

	return retCol;
}

std::ostream& operator<<(std::ostream& os, const MonotonicityStructure& ms) 
{
	os << ms.extremePoints;
	return os;
}

//////////////////////////////////////////////////////////////////////////////
//
// Lattice Point
//
//////////////////////////////////////////////////////////////////////////////

bool LatticePoint::operator==(const LatticePoint &y) const {
	
	if (point.size() != y.point.size()) {
		return false;
	}
	for (int i = 0; i < point.size(); ++i) {
		if (point[i] != y.point[i]) {
			return false;
		}
	}
	return true;
}

bool LatticePoint::operator<(const LatticePoint &y) const {

	if (point.size() != y.point.size()) {
		return false;
	}

	bool prev_coord_less_than = false;

	for (int i = 0; i < point.size(); ++i) {
		if (point[i] == y.point[i]) {
			continue;
		}
		else if (point[i] < y.point[i]) {
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

int LatticePoint::dimension() const {
	return point.size();
}

int LatticePoint::product() const {
	return std::accumulate(std::begin(point), std::end(point), 1, std::multiplies<int>());
}

std::ostream& operator<<(std::ostream& os, const LatticePoint& latticept)
{
	os << '(';
	auto d = latticept.point.size();
	for (int i = 0; i < d; ++i) {
		os << latticept.point[i] << ',';
	}
	os << ')';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<LatticePoint>& support)
{
	for (auto && sup : support) {
		os << sup << '\n';
	}
	return os;
}

//////////////////////////////////////////////////////////////////////////////
//
// Discrete Measure
//
//////////////////////////////////////////////////////////////////////////////

DiscreteMeasure DiscreteMeasure::operator+(const DiscreteMeasure &other_dm) 
{
	DiscreteMeasure dm_new {this->support, this->weights};
	dm_new += other_dm;
	return dm_new;
}

DiscreteMeasure& DiscreteMeasure::operator+=(const DiscreteMeasure &other_em)
{
	for(int i = 0; i < other_em.size(); ++i) {
		// note : may have to implement custom comparator should numerical issues arise
		auto found = std::find(std::begin(support), std::end(support), other_em.support[i]);

		if (found != std::end(support)) {
			// found in this support
			int index = std::distance(std::begin(support),found);
			weights[index] += other_em.weights[i];
			continue;
		}
		// add point
		support.push_back(other_em.support[i]);
		weights.push_back(other_em.weights[i]);
	}

	// elements were added; sort!
	sort();

	return *this;
}

int DiscreteMeasure::dimension() const noexcept{
	return support[0].dimension();
}

int DiscreteMeasure::size() const noexcept {
	return support.size();
}

void DiscreteMeasure::sort() noexcept {
	auto sorted_indices = sort_indices(this->support);	
	std::sort(this->support.begin(), this->support.end());
	reorder(this->weights, sorted_indices);
}

//////////////////////////////////////////////////////////////////////////////
//
// Extreme Measure
//
//////////////////////////////////////////////////////////////////////////////

// FINISH-ME
ExtremeMeasure ExtremeMeasure::marginalize(const std::vector<int> &to_marginalize_out) const
{
	// note : to_marginalize_out contains indices we want to collapse and assumes 0-indexing

	// orig:  0 1 2 3 4 5
	// t_m_o : 0 2 4

	// start with 0:
	// orig_1 : 1 2 3 4 5 (actually "0 1 2 3 4")
	// t_m_o : 2 4 (actually "1 3")

	// now remove original 2 but now "1"
	// orig_2 : 1 3 4 5 (actually "0 2 3 4", actually "0 1 2 3")
	// orig t_m_o : 4 (actually "2")

	// now remove original 4 but now "2"
	// orig_3 : 1 3 5 (actually "0, 1, 2")

	// get rid of the assert and return this pointer / copy?
	assert(to_marginalize_out.size() > 0);
	assert(to_marginalize_out.size() < dimension());	// TOTEST

	// update monotone structure
	auto updated_ms = monotone_structure;

	// marginalized support
	// marginalized weightsa
}

std::ostream& operator<<(std::ostream& os, const ExtremeMeasure& em) 
{
	os <<  AnsiColor::magenta << "Extreme Measure:" << AnsiColor::none << '\n';
	os << AnsiColor::green << "Monotone Structure: " << AnsiColor::none;
	PrettyPrint(em.monotone_structure); 
	os << AnsiColor::green << "Dimensions: " << AnsiColor::none << em.dimension() << '\n';
	os << AnsiColor::green <<"Support: " << AnsiColor::none << '\n';
	os << em.support; 
	os << AnsiColor::green <<"Weights: " << AnsiColor::none << '\n';
	PrettyPrint(em.weights);
	os << AnsiColor::green <<"Means: " << AnsiColor::none << '\n';
	PrettyPrint(em.means);
	os << AnsiColor::green <<"Variances: " << AnsiColor::none << '\n';
	PrettyPrint(em.variances);
	return os;
}

// convenience function
ExtremeMeasures construct_Poisson_ExtremeMeasures(const std::vector<double>& intensities)
{
	// construct EmpDistrArray
	auto poiss_emdistr_array = construct_Poisson_EmpDistrArray(intensities);
	const int dim = intensities.size();
	// generate monotone structures
	const auto ms = MonotonicityStructure(dim);
	const int num_ms = ms.num_extremepts();

	ExtremeMeasures ems;
	ems.reserve(num_ms);

	for (int i = 0; i < num_ms; ++i) {
		ems.emplace_back( ejd( poiss_emdistr_array, ms[i] ) );
		for(int j = 0; j < intensities.size(); ++j) {
			const int this_intensity = intensities[j];
			ems[i].means.push_back(this_intensity);
			ems[i].variances.push_back(this_intensity);
		}
	}
	return ems;
}

//////////////////////////////////////////////////////////////////////////////
//
// Extreme Joint Distribution
//
//////////////////////////////////////////////////////////////////////////////

// TODO refactor and split into two sub functions (into more general utilities)
std::vector<std::vector<double>> flip_EmpDistrArray_CDF(
	const std::vector<EmpiricalDistribution>& marginal_pdfs,
	const std::vector<int>& monotone_struture) 
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
		[] (auto & z) {
			apply_cumsum(&z);
		}
	);
	return marginal_cdf;
}

std::vector<std::vector<double>> flip_supports(
	const std::vector<EmpiricalDistribution>& marginal_pdfs,
	const std::vector<int>& monotone_structs)
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
	return { {.support=support, .weights=weights}, .monotone_structure=monotone_structs};
}
// namespace ejd	
}
