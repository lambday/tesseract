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
#include <tesseract/regularizer/SpectralVariance.hpp>

using namespace tesseract;

template <template <class> class Regularizer, typename T>
ComputeFunction<Regularizer, T>::ComputeFunction() : eta(default_eta)
{
}

template <template <class> class Regularizer, typename T>
const T ComputeFunction<Regularizer, T>::operator ()(const Eigen::Ref<const Matrix<T>>& cov) const
{
	index_t N = cov.rows() - 1;

	// evaluate the squared multiple correlation which is b_S.C_S^{-1} b_S
	T R_sq = 0;

	// avoid nan values when the C_S matrix is singular
	// TODO think of using Moore-Penrose pseudo-inverse
	if (cov.topLeftCorner(N, N).diagonal().minCoeff() > std::numeric_limits<float64_t>::epsilon())
	{
		auto& b_S = cov.rightCols(1).topRows(N);
		Eigen::Map<Vector<T>> b_S_map(const_cast<float64_t*>(b_S.data()), N);
		R_sq = b_S_map.dot(cov.topLeftCorner(N, N).llt().solve(b_S_map));
	}

	// compute the regularizer on C_S
	Regularizer<T> regularizer;
	regularizer.set_params(reg_params);
	T f = regularizer(cov.topLeftCorner(N, N));

	return R_sq + eta * f;
}

template <template <class> class Regularizer, typename T>
void ComputeFunction<Regularizer, T>::set_eta(T _eta)
{
	eta = _eta;
}

template <template <class> class Regularizer, typename T>
void ComputeFunction<Regularizer, T>::set_reg_params(typename
		ComputeFunction<Regularizer,T>::reg_param_type _reg_params)
{
	reg_params = _reg_params;
}

template class ComputeFunction<DummyRegularizer, float64_t>;
template class ComputeFunction<SmoothedDifferentialEntropy, float64_t>;
template class ComputeFunction<SpectralVariance, float64_t>;
