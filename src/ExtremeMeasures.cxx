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
