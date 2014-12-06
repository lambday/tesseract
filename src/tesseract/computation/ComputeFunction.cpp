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

using namespace tesseract;

template <template <class> class Regularizer, typename T>
ComputeFunction<Regularizer, T>::ComputeFunction() : eta(static_cast<T>(0.5))
{
}

template <template <class> class Regularizer, typename T>
ComputeFunction<Regularizer, T>::ComputeFunction(T _eta) : eta(_eta)
{
}

template <template <class> class Regularizer, typename T>
const T ComputeFunction<Regularizer, T>::operator ()(const Matrix<T>& X) const
{
	// compute covariance which is X^TX since the columns of the matrices have unit norm
	Matrix<T> cov = X.transpose() * X;
	index_t N = cov.rows() - 1;

	// evaluate the squared multiple correlation which is b_S.C_S^1 b_S
	T R_sq = 0;

	// avoid nan values when the C_S matrix is singular
	if (cov.block(0,0,N,N).diagonal().minCoeff() > 0.0)
	{
		Vector<T> b_S = cov.block(0,N,N,1);
		R_sq = b_S.dot(cov.block(0,0,N,N).llt().solve(b_S));
	}

	// compute the regularizer
	Regularizer<T> regularizer;
	T f = regularizer(cov);

	return R_sq + eta * f;
}

template class ComputeFunction<DummyRegularizer, float64_t>;
template class ComputeFunction<SmoothedDifferentialEntropy, float64_t>;
