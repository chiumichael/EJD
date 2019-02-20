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

#include "ExtremeMeasures.hpp"

namespace ejd {

// TO-DO : implement the non-recursive version
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

// namespace ejd	
}
