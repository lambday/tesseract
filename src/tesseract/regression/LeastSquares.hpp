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

#ifndef LEAST_SQUARES_H__
#define LEAST_SQUARES_H__

#include <tesseract/base/types.h>

namespace tesseract
{

enum LeastSquareMethod
{
	LS_SVD,
	LS_QR,
	LS_NORMAL
};

template <class Matrix, class Vector, enum LeastSquareMethod>
struct LeastSquares;

template <class Matrix, class Vector>
struct LeastSquares<Matrix, Vector, LS_SVD>
{
	void solve(const Matrix& A, const Vector& b, Vector& result) const
	{
		result = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
	}
};

template <class Matrix, class Vector>
struct LeastSquares<Matrix, Vector, LS_QR>
{
	void solve(const Matrix& A, const Vector& b, Vector& result) const
	{
		result = A.colPivHouseholderQr().solve(b);
	}
};

template <class Matrix, class Vector>
struct LeastSquares<Matrix, Vector, LS_NORMAL>
{
	void solve(const Matrix& A, const Vector& b, Vector& result) const
	{
		result = (A.transpose() * A).ldlt().solve(A.transpose() * b);
	}
};

}

#endif // LEAST_SQUARES_H__
