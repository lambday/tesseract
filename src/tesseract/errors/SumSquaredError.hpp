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

#ifndef SUM_SQUARED_ERROR_H__
#define SUM_SQUARED_ERROR_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief computes the sum of squares error as \f$\sigma=\sqrt{\frac{(Z-Z')^2}{N-2}}\f$.
 * where \f$Z\f$ is the labels and \f$Z'\f$ is the prediction according to regression
 * model.
 */
template <typename T>
struct SumSquaredError
{
	/**
	 * @param Z the original regressands \f$Z\f$
	 * @param Zp the predicted regressands according to model \f$Z'\f$
	 * @return the sum of squares error (see class documentation)
	 */
	float64_t compute(const Eigen::Ref<const Vector<T>>& Z, const Eigen::Ref<const Vector<T>>& Zp);
};

}

#endif // SUM_SQUARED_ERROR_H__
