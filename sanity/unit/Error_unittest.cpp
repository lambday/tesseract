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

#include <tesseract/errors/SumSquaredError.hpp>
#include <tesseract/errors/PearsonsCorrelation.hpp>
#include <iostream>

using namespace tesseract;
using namespace Eigen;

// tested from http://onlinestatbook.com/2/regression/accuracy.html
void test1()
{
	VectorXd Z(5);
	Z << 1.0, 2.0, 1.3, 3.75, 2.25;
	VectorXd Zp(5);
	Zp << 1.210, 1.635, 2.060, 2.485, 2.910;

	SumSquaredError<float64_t> error;
//	std::cout << error.compute(Z, Zp) << std::endl;
	// expected result 0.747 (for population) and 0.964 (for sample)
}

// tested from http://onlinestatbook.com/2/regression/accuracy.html
void test2()
{
	VectorXd Z(5);
	Z << 1.0, 2.0, 1.3, 3.75, 2.25;
	VectorXd Zp(5);
	Zp << 1.210, 1.635, 2.060, 2.485, 2.910;

	PearsonsCorrelation<float64_t> error;
//	std::cout << error.compute(Z, Zp) << std::endl;
	// expected result 0.747 (for population) and 0.964 (for sample)
}

int main(int argc, char** argv)
{
	test1();
	test2();
	return 0;
}
