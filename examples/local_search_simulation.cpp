/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Soumyajit De
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <tesseract/base/init.hpp>
#include <tesseract/algorithm/LocalSearch.hpp>
#include <tesseract/algorithm/LinearLocalSearch.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <chrono>

using namespace tesseract;
using namespace Eigen;

void test1(int dim)
{
	int N = 1000;

	// generate random dataset
	MatrixXd m = MatrixXd::Random(N, dim+1);
	m.colwise().normalize();

	index_t max_zero_cols = dim / 3;

	// randomly add zero columns
	for (index_t i = 0; i < max_zero_cols; ++i)
	{
		index_t j = rand() % (dim - 1);
//		std::cout << j << std::endl;
		m.col(j) = VectorXd::Zero(N);
	}

	MatrixXd cov = m.transpose() * m;

	if (logger.get_loglevel() >= MemDebug)
		logger.print_matrix(cov);

	LocalSearch<SmoothedDifferentialEntropy, float64_t> ls(cov);
	typedef LocalSearch<SmoothedDifferentialEntropy, float64_t>::param_type ls_param_type;
	ls.set_params(ls_param_type());

	auto cpu0 = std::chrono::high_resolution_clock::now();
	std::pair<float64_t,std::vector<index_t>> res_1 = ls.run();
	auto cpu1 = std::chrono::high_resolution_clock::now();
	auto duration1 = cpu1 - cpu0;

	LinearLocalSearch<SmoothedDifferentialEntropy, float64_t> lls(cov);
	typedef LinearLocalSearch<SmoothedDifferentialEntropy, float64_t>::param_type lls_param_type;
	lls.set_params(lls_param_type());

	auto cpu2 = std::chrono::high_resolution_clock::now();
	std::pair<float64_t,std::vector<index_t>> res_2 = lls.run();
	auto cpu3 = std::chrono::high_resolution_clock::now();
	auto duration2 = cpu3 - cpu2;

	printf("%u %u %f %f %f %f\n", res_1.second.size(), res_2.second.size(), res_1.first, res_2.first,
			std::chrono::duration<float64_t>(duration1).count(),
			std::chrono::duration<float64_t>(duration2).count());
}

int main(int argc, char** argv)
{
//	logger.set_loglevel(MemDebug);

	for (index_t i = 10; i < 200; ++i)
		test1(i);
	return 0;
}
