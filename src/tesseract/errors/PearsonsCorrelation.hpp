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

#ifndef PEARSONS_CORRELATION_H__
#define PEARSONS_CORRELATION_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief computes the Pearson's correlation as \f$\sigma=\sqrt{\frac{(1-r^2)SSY}
 * {N-2}}\f$ where \f$\text{SSY}=\sum{(Z-\bar{Z})^2}\f$, \f$Z\f$ is the original labels,
 * \f$\bar{Z}=\frac{\sum{Z_i}}{N}\f$, \f$r=\frac{1}{N}\sum_{i=1}^N\left(\frac{Z_i-\bar{Z}}
 * {s_Z} \right )\left(\frac{Z'_i-\bar{Z'}}{s_{Z'}} \right )\f$ and \f$s_Z=\sqrt{\frac{1}
 * {N-1}\sum_{i=1}^N\left(Z_i-\bar{Z}\right )^2}\f$
 */
template <typename T>
struct PearsonsCorrelation
{
	/**
	 * @param Z the original regressands \f$Z\f$
	 * @param Zp the predicted regressands according to model \f$Z'\f$
	 * @return Pearson's correlation (see class documentation)
	 */
	float64_t compute(const Vector<T>& Z, const Vector<T>& Zp);
};

}

#endif // PEARSONS_CORRELATION_H__
