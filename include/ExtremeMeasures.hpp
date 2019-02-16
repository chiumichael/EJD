#pragma once

// eigen
#include <Eigen/Core>

namespace ejd {

struct MonotonicityStructure {
	explicit MonotonicityStructure(const int dim);
	int num_extremepts() const;
	Eigen::MatrixXi extremePts_;
};

// namespace ejd
}
