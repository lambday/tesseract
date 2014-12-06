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

#ifndef COMPUTE_FUNCTION_H__
#define COMPUTE_FUNCTION_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief template class ComputeFunction for computing the objective function
 */
template <template <class> class Regularizer, typename T>
struct ComputeFunction
{
	/** regularizer param */
	typedef typename Regularizer<T>::param_type reg_param_type;

	/** default constructor */
	ComputeFunction();

	/*
	 * @param X the regressors (real valued dense feature matrix) and
	 * the regressand (the last column)
	 * @return the function value
	 */
	const T operator ()(const Matrix<T>& X) const;

	/** @param _eta regularization constant */
	void set_eta(T _eta);

	/** @param _param regularizer params */
	void set_reg_params(reg_param_type _reg_params);

	/** regularization constant \f$\eta > 0\f$ */
	T eta;

	/** regularizer params */
	reg_param_type reg_params;

	/** default value of eta = 0.5 */
	static constexpr T default_eta = static_cast<T>(0.5);
};

}

#endif // COMPUTE_FUNCTION_H__
