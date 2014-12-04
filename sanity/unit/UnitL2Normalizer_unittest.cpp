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

#include <tesseract/normalizer/UnitL2Normalizer.hpp>
#include <iostream>

using namespace tesseract;
using namespace Eigen;

void test1()
{
	MatrixXd m = MatrixXd::Random(4,3);
//	std::cout << m.norm() << std::endl;

	UnitL2Normalizer<MatrixXd> normalizer;
	normalizer.normalize(m);
//	std::cout << m.norm() << std::endl;
}

void test2()
{
	VectorXd m = VectorXd::Random(4);
//	std::cout << m.norm() << std::endl;

	UnitL2Normalizer<VectorXd> normalizer;
	normalizer.normalize(m);
//	std::cout << m.norm() << std::endl;
}

int main(int argc, char** argv)
{
	test1();
	test2();
	return 0;
}
