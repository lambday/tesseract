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

#include <tesseract/computation/ComputeFunction.hpp>
#include <tesseract/regularizer/DummyRegularizer.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <cmath>

using namespace tesseract;
using namespace Eigen;

void test1()
{
	int dim = 2;
	int n = 3;
	MatrixXd m(n, dim + 1);

	m << 0.68037, 0.59688, -0.32955,
		-0.21123, 0.82329, 0.53646,
		0.56620, -0.60490, -0.44445;
	ComputeFunction<DummyRegularizer, float64_t> f;
	float64_t val = f(m.transpose() * m);

	// compute it another way
	MatrixXd A = m.block(0, 0, n, dim);
	VectorXd b = m.col(m.cols()-1);

	// compute by solving least squares
	VectorXd beta = (A.transpose() * A).ldlt().solve(A.transpose() * b);
	float val2 = 1 - pow((b - A*beta).norm(),2);

	// since we are using dummy regularizer so these two values should be same
	// in fact is should be 0.936133 (computed using octave)
	assert(abs(val - val2) < std::numeric_limits<float64_t>::epsilon());
}

void test2()
{
	int dim = 2;
	int n = 3;
	MatrixXd m(n, dim + 1);

	m << 0.68037, 0.59688, -0.32955,
		-0.21123, 0.82329, 0.53646,
		0.56620, -0.60490, -0.44445;

	float64_t eta = ComputeFunction<SmoothedDifferentialEntropy, float64_t>::default_eta;
	float64_t delta = SmoothedDifferentialEntropyParam<float64_t>::default_delta;

	typedef SmoothedDifferentialEntropy<float64_t>::param_type reg_param_type;

	ComputeFunction<SmoothedDifferentialEntropy, float64_t> f;
	f.set_eta(eta);
	f.set_reg_params(reg_param_type(delta));
	MatrixXd cov = m.transpose() * m;
	float64_t val = f(cov);

	// compute it another way
	MatrixXd A = m.block(0, 0, n, dim);
	VectorXd b = m.col(m.cols()-1);

	// compute by solving least squares
	VectorXd beta = (A.transpose() * A).ldlt().solve(A.transpose() * b);
	float64_t val2 = 1 - pow((b - A*beta).norm(),2);

	// compute the regularizer

	SmoothedDifferentialEntropy<float64_t> r;
	r.set_params(reg_param_type(delta));
	float64_t val3 = r(cov.topLeftCorner(dim, dim));

	assert(abs(val - (val2 + eta * val3)) < std::numeric_limits<float64_t>::epsilon());
}

void test3()
{
	int dim = 2;
	int n = 3;
	MatrixXd m(n, dim + 1);

	m << 0.68037, 0.59688, -0.32955,
		-0.21123, 0.82329, 0.53646,
		0.56620, -0.60490, -0.44445;

	float64_t eta = 0.8;
	float64_t delta = 0.1;

	typedef SmoothedDifferentialEntropy<float64_t>::param_type reg_param_type;

	ComputeFunction<SmoothedDifferentialEntropy, float64_t> f;
	f.set_eta(eta);
	f.set_reg_params(reg_param_type(delta));
	MatrixXd cov = m.transpose() * m;
	float64_t val = f(cov);

	// compute it another way
	MatrixXd A = m.block(0, 0, n, dim);
	VectorXd b = m.col(m.cols()-1);

	// compute by solving least squares
	VectorXd beta = (A.transpose() * A).ldlt().solve(A.transpose() * b);
	float64_t val2 = 1 - pow((b - A*beta).norm(),2);

	// compute the regularizer

	SmoothedDifferentialEntropy<float64_t> r;
	r.set_params(reg_param_type(delta));
	float64_t val3 = r(cov.topLeftCorner(dim, dim));

	assert(abs(val - (val2 + eta * val3)) < std::numeric_limits<float64_t>::epsilon());
}

void test4()
{
	int dim = 3;
	int n = 5;
	MatrixXd m(n, dim + 1);

	m << -0.620251,   0.043870,   0.126634,   0.883738,
		-0.649199,  -0.156014,   0.025161,  -0.116296,
		-0.179758,  -0.387849,   0.067516,   0.270564,
		0.256471,  -0.463623,  -0.604798,  -0.161666,
		-0.309417,   0.779976,   0.782938,   0.325794;
	ComputeFunction<DummyRegularizer, float64_t> f;
	float64_t val = f(m.transpose() * m);

	// octave computed value from the following code
	/*
	   X = m(:,linspace(1, dim, dim))
	   y = m(:,[dim + 1])
	   C = m'*m
	   C_S = C(linspace(1, dim, dim),linspace(1, dim, dim))
	   b_S = C(linspace(1, dim, dim),[dim + 1])
	   R_sq = dot(b_S, C_S \ b_S)
	*/
	assert(abs(val - 0.46923) < 1E-6);
}

int main(int argc, char** argv)
{
	test1();
	test2();
	test3();
	test4();
	return 0;
}
