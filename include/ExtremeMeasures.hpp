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

// 3rd party
#include <Eigen/Core>
// stl
#include <algorithm>
#include <vector>

namespace ejd {

// fwd declarations
struct EmpiricalDistribution;
struct EmpDistrArray;

//////////////////////////////////////////////////////////////////////////////
//
// Monotonicity Structure
//
//////////////////////////////////////////////////////////////////////////////

struct MonotonicityStructure {
	explicit MonotonicityStructure(const int dim);
	int num_extremepts() const;
	Eigen::MatrixXi extremePts_;
};

//////////////////////////////////////////////////////////////////////////////
//
// Extreme Measures
//
//////////////////////////////////////////////////////////////////////////////

struct LatticePoint {
    std::vector<int> point_;
    // comparison operators 
    bool operator==(const LatticePoint & y) const;
    bool operator<(const LatticePoint & y) const;
    // constructors
    LatticePoint(const std::vector<int> point) 
        : point_(std::move(point))
    {}
};

std::ostream& operator<<(std::ostream& os, const LatticePoint& point);

struct ExtremeMeasure
{
    std::vector<LatticePoint> support;
    std::vector<double> weights;
    std::vector<int> monotone_structure;
    // methods
    ExtremeMeasure operator+() const;
    int dimension() const;
};

//////////////////////////////////////////////////////////////////////////////
//
// Extreme Joint Distribution
//
//////////////////////////////////////////////////////////////////////////////

template <typename T>
void apply_cumsum(std::vector<T> * v_ptr) 
{
    std::vector<T> & v = * v_ptr;
    for (int i = 1; i < v.size(); i++) {
        v[i] += v[i-1];
    }
}

template <typename T>
void apply_cumsum(std::vector<std::vector<T>> * v_ptr)
{
    std::vector<std::vector<T>> & v = * v_ptr;
    std::for_each(v.begin(), v.end(),
        [] (std::vector<T> v_in) {
            apply_cumsum(v_in);
        }
    );
}

template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T>>& v) {
    std::size_t total_size = 0;
    for (const auto & sub : v) {
        total_size += sub.size();
    }
    std::vector<T> result;
    result.reserve(total_size);
    for (const auto & sub : v) {
        result.insert(result.end(), sub.begin(), sub.end());
    }
    return result;
}

template <typename T>
std::vector<T> flip_vectors(const std::vector<T> vector_to_flip, const std::vector<int> flip_indicator) 
{
    std::vector<T> flipped_vector;

    for (int i = 0; i < flipped_vector.size(); ++i) {
        
        flipped_vector.push_back(vector_to_flip[i]);

        if (flip_indicator[i] == -1) {
            std::reverse(std::begin(flipped_vector[i]), std::end(flipped_vector[i]));
        }
    }
    return flipped_vector;
}

std::vector<std::vector<double>> flip_EmpDistrArray_CDF(
    const std::vector<EmpiricalDistribution> marginal_pdfs,
    const std::vector<int> monotone_struture);

std::vector<std::vector<double>> flip_supports(
    const std::vector<EmpiricalDistribution> marginal_pdfs,
    const std::vector<int> monotone_structs);

void ensure_right_tail(std::vector<double> * prob_distr_);

ExtremeMeasure ejd(EmpDistrArray empdistrarrs, std::vector<int> monotone_structs);

// namespace ejd
}
