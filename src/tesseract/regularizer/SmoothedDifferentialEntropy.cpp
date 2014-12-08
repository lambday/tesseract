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
#include <cmath>

using namespace tesseract;

template <typename T>
SmoothedDifferentialEntropyParam<T>::SmoothedDifferentialEntropyParam()
: delta(default_delta)
{
}

template <typename T>
SmoothedDifferentialEntropyParam<T>::SmoothedDifferentialEntropyParam(T _delta)
: delta(_delta)
{
}

template <typename T>
SmoothedDifferentialEntropy<T>::SmoothedDifferentialEntropy()
{
}

template <typename T>
SmoothedDifferentialEntropy<T>::~SmoothedDifferentialEntropy()
{
}

template <typename T>
const T SmoothedDifferentialEntropy<T>::operator()(const Eigen::Ref<const Matrix<T>>& cov) const
{
	// compute the eigen values
	Vector<T> eigenvalues = cov.eigenvalues().real();
	index_t k = eigenvalues.rows();

	T inv_log_2 = static_cast<T>(1.0 / log(2));
	std::for_each(eigenvalues.data(), eigenvalues.data() + k, [this, inv_log_2](T& val)
	{
		val = log(this->params.delta + val) * inv_log_2;
	});

	return eigenvalues.array().sum() - 3 * k * log(params.delta) * inv_log_2;
}

template <typename T>
void SmoothedDifferentialEntropy<T>::set_params(typename
		SmoothedDifferentialEntropy<T>::param_type _params)
{
	params = _params;
}

template class SmoothedDifferentialEntropyParam<float64_t>;
template class SmoothedDifferentialEntropy<float64_t>;
