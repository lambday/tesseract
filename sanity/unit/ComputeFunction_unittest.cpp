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
	float64_t val = f(m);

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

int main(int argc, char** argv)
{
	test1();
	return 0;
}
