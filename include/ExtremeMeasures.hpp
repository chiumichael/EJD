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
#include <blaze/math/DynamicMatrix.h>
// stl
#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>

namespace ejd {

namespace b = blaze;

// fwd declarations
struct EmpiricalDistribution;
struct EmpDistrArray;

//////////////////////////////////////////////////////////////////////////////
//
// Monotonicity Structure
//
//////////////////////////////////////////////////////////////////////////////

struct MonotonicityStructure
{
    b::DynamicMatrix<int> extremePoints;
    explicit MonotonicityStructure(const int dim);
	int num_extremepts() const;
    std::pair<int,int> size() const;
    std::vector<int> operator[](int col) const;
};

std::ostream& operator<<(std::ostream& os, const MonotonicityStructure& ms);

//////////////////////////////////////////////////////////////////////////////
//
// LatticePoint
//
//////////////////////////////////////////////////////////////////////////////

// note : multidimensional
// TODO : implement a span for views into point for marginalization
struct LatticePoint  
{
    std::vector<int> point;
    // comparison operators 
    bool operator==(const LatticePoint & y) const;
    bool operator<(const LatticePoint & y) const;
    // constructors
    LatticePoint(const std::vector<int> point) 
        : point(std::move(point))
    {}
    // methods
    int dimension() const;
    int product() const;
};

std::ostream& operator<<(std::ostream& os, const LatticePoint& point);

//////////////////////////////////////////////////////////////////////////////
//
// Discrete Measure
//
//////////////////////////////////////////////////////////////////////////////

struct DiscreteMeasure 
{
    std::vector<LatticePoint> support;
    std::vector<double> weights;
    // operators
    DiscreteMeasure operator+(const DiscreteMeasure& other_dm);
    DiscreteMeasure& operator+=(const DiscreteMeasure& other_dm);
    // methods
    int dimension() const noexcept;
    int size() const noexcept;
    // ExtremeMeasure marginalize(const std::vector<int> to_marginalize_out) const;
protected:
    void sort() noexcept;
};

//////////////////////////////////////////////////////////////////////////////
//
// Extreme Measure
//
//////////////////////////////////////////////////////////////////////////////

// TODO : move elsewhere...
template <typename T>
std::vector<size_t> sort_indices(const std::vector<T>& v) {
    // original indices
    std::vector<size_t> idx(v.size());
    std::iota(std::begin(idx), std::end(idx), 0);

    // sort 
    std::sort(
        std::begin(idx), std::end(idx),
        [&v] (size_t i1, size_t i2) {
            return v[i1] < v[i2];
        }
    );
    return idx;
}

// O(n) time and O(1) algorithm to reorder given target indices
template <typename T>
void reorder(std::vector<T>& v, std::vector<size_t>& index)
{
    assert(v.size() == index.size());
    for(int i = 0; i < index.size(); ++i) 
    {
        while(index[i] != i) {
            size_t this_index = index[i];
            int old_index = index[this_index];
            T old_value = v[this_index];
            
            v[this_index] = v[i];
            index[this_index] = index[i];

            index[i] = old_index;
            v[i] = old_value;
        }
    }
}

// TODO : implement move and copy constructors
// multidimensional
struct ExtremeMeasure : public DiscreteMeasure
{
    std::vector<int> monotone_structure;
    std::vector<double> means;
    std::vector<double> variances;
    ExtremeMeasure marginalize(const std::vector<int>& to_marginalize_out) const;
private:
    void sort() noexcept;
};

// typedefs
using ExtremeMeasures = std::vector<ExtremeMeasure>;

std::ostream& operator<<(std::ostream& os, const ExtremeMeasure& em);

ExtremeMeasures construct_Poisson_ExtremeMeasures(const std::vector<double>& intensities);

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
std::vector<T> flip_vectors(const std::vector<T>& vector_to_flip, const std::vector<int>& flip_indicator)
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
    const std::vector<EmpiricalDistribution>& marginal_pdfs,
    const std::vector<int>& monotone_struture);

std::vector<std::vector<double>> flip_supports(
    const std::vector<EmpiricalDistribution>& marginal_pdfs,
    const std::vector<int>& monotone_structs);

void ensure_right_tail(std::vector<double> * prob_distr_);

ExtremeMeasure ejd(EmpDistrArray empdistrarrs, std::vector<int> monotone_structs);

// namespace ejd
}
