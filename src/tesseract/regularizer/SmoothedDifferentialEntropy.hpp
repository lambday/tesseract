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

#ifndef SMOOTHED_DIFFERENTIAL_ENTROPY_H__
#define SMOOTHED_DIFFERENTIAL_ENTROPY_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief class SmoothedDifferentialEntropy for a computing a smoothed differential
 * entropy as \f$\sum_{i=1}^{|S|}{\log_2(\delta+\lambda_i(C_S))}-3k\log_2\delta\f$
 * where \f$\delta>0\f$ is a smoothing constant.
 */
template <typename T>
struct SmoothedDifferentialEntropy
{
	/** default constructor (sets smoothing constant to 1) */
	SmoothedDifferentialEntropy();

	/** constructor
	 * @param delta the smoothing constant
	 */
	SmoothedDifferentialEntropy(T _delta);

	/** destructor */
	~SmoothedDifferentialEntropy();

	/**
	 * @param cov the covariance matrix
	 * @return the regularizer value
	 */
	const T operator()(const Matrix<T>& cov) const;

	/** smoothing constant delta */
	T delta;
};

}

#endif // SMOOTHED_DIFFERENTIAL_ENTROPY_H__
