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

#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace tesseract;
using namespace Eigen;

void test1()
{
	int dim = 2;
	int n = 3;
	MatrixXd m(n, dim + 1);

	m <<
	0.539426,   0.097298,   0.343196,
	0.602504,   0.478170,   0.685719,
	0.588226,   0.872861,   0.641877;

	SmoothedDifferentialEntropy<float64_t> f;
	typedef SmoothedDifferentialEntropy<float64_t>::param_type param_type;
	f.set_params(param_type(1));
	float64_t val = f(m.transpose() * m);

	assert(abs(val - 2.1616) < 1E-6);
}

int main(int argc, char** argv)
{
	test1();
	return 0;
}
