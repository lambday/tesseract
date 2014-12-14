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

#ifndef SPECTRAL_VARIANCE_H__
#define SPECTRAL_VARIANCE_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief SpectralVariance params */
template <typename T>
struct SpectralVarianceParam
{
};

/** @brief class SpectralVariance
 */
template <typename T>
struct SpectralVariance
{
	/** param type */
	typedef SpectralVarianceParam<T> param_type;

	/** default constructor */
	SpectralVariance();

	/** destructor */
	~SpectralVariance();

	/**
	 * @param cov the covariance matrix
	 * @return the regularizer value
	 */
	const T operator()(const Eigen::Ref<const Matrix<T>>& cov) const;

	/** @param _param the regularizer param */
	void set_params(param_type _params);

	/** smoothing constant delta */
	param_type params;

};

}

#endif // SPECTRAL_VARIANCE_H__
