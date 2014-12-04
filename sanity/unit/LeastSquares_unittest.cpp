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

#include <iostream>
#include <tesseract/regression/LeastSquares.hpp>
#include <Eigen/Eigen>

using namespace tesseract;
using namespace Eigen;
using namespace std;

template <class Matrix, class Vector>
void test_svd(const Matrix& A, const Vector& b, Vector& x)
{
	LeastSquares<Matrix, Vector, LS_SVD> ls;
	ls.solve(A, b, x);
//	cout << "beta = " << endl << x << endl;
//	cout << "residual = " << b-A*x << endl;
}

template <class Matrix, class Vector>
void test_qr(const Matrix& A, const Vector& b, Vector& x)
{
	LeastSquares<Matrix, Vector, LS_QR> ls;
	ls.solve(A, b, x);
//	cout << "beta = " << endl << x << endl;
//	cout << "residual = " << b-A*x << endl;
}

template <class Matrix, class Vector>
void test_normal(Matrix& A, Vector& b, Vector& x)
{
	LeastSquares<Matrix, Vector, LS_NORMAL> ls;
	ls.solve(A, b, x);
//	cout << "beta = " << endl << x << endl;
//	cout << "residual = " << b-A*x << endl;
}

int main(int argc, char** argv)
{
	MatrixXd A = MatrixXd::Random(3, 2);
	VectorXd b = VectorXd::Random(3);
	VectorXd res = VectorXd::Zero(2);

//	cout << "A = " << endl << A << endl;
//	cout << "b = " << endl << b << endl;

	test_svd(A, b, res);
	test_qr(A, b, res);
	test_normal(A, b, res);

	Map<MatrixXd> map_A(A.data(), A.rows(), A.cols());
	Map<VectorXd> map_b(b.data(), b.rows());
	Map<VectorXd> map_res(res.data(), res.rows());

	test_svd(map_A, map_b, map_res);
	test_qr(map_A, map_b, map_res);
	test_normal(map_A, map_b, map_res);

	return 0;
}
