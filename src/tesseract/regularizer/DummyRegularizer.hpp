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

#ifndef DUMMY_REGULARIZER_H__
#define DUMMY_REGULARIZER_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief dummy regularizer param */
struct DummyRegularizerParam
{
};

/** @brief class DummyRegularizer for a dummy regularizer which does nothing
 * and has a compute method which returns 0. Simulates the situation with no
 * regularizer at all.
 */
template <typename T>
struct DummyRegularizer
{
	/** parameter type */
	typedef DummyRegularizerParam param_type;

	/**
	 * @param cov the covariance matrix
	 * @return the regularizer value (always 0 in this case)
	 */
	const T operator()(const Eigen::Ref<const Matrix<T>>& cov) const;

	/** @param _param the regularizer param */
	void set_params(param_type params);
};

}

#endif // DUMMY_REGULARIZER_H__
