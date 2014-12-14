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
#include <cmath>

using namespace tesseract;

template <typename T>
SpectralVariance<T>::SpectralVariance()
{
}

template <typename T>
SpectralVariance<T>::~SpectralVariance()
{
}

template <typename T>
const T SpectralVariance<T>::operator()(const Eigen::Ref<const Matrix<T>>& cov) const
{
	// compute the eigen values
	Vector<T> eigenvalues = cov.eigenvalues().real();
	index_t k = eigenvalues.rows();

	std::for_each(eigenvalues.data(), eigenvalues.data() + k, [](T& val)
	{
		val = pow(val - 1, 2);
	});

	return 9 * k * k - eigenvalues.array().sum();
}

template <typename T>
void SpectralVariance<T>::set_params(typename
		SpectralVariance<T>::param_type _params)
{
	params = _params;
}

template class SpectralVarianceParam<float64_t>;
template class SpectralVariance<float64_t>;
