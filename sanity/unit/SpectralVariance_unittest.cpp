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

#include <tesseract/regularizer/SpectralVariance.hpp>
#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace tesseract;
using namespace Eigen;

void test1()
{
	int dim = 3;
	int n = 3;
	MatrixXd m(n, dim);

	m <<
	0.47879, 0.14849, 0.69439,
	0.68805, 0.79288, 0.49378,
	0.54530, 0.59101, 0.52346;

	SpectralVariance<float64_t> f;
	typedef SpectralVariance<float64_t>::param_type param_type;
	f.set_params(param_type());
	MatrixXd cov = m.transpose() * m;
	float64_t val = f(cov);

	assert(abs(val - 76.110) < 1E-6);
}

int main(int argc, char** argv)
{
	test1();
	return 0;
}
