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

#ifndef FORWARD_REGRESSION_H__
#define FORWARD_REGRESSION_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief struct for parameters used in forward-regression */
template <template <class> class Regularizer, typename T>
struct ForwardRegressionParam
{
	/** regularizer param type */
	typedef typename Regularizer<T>::param_type param_type;

	/** default constructor */
	ForwardRegressionParam();

	/** constructor */
	ForwardRegressionParam(T _eta, param_type reg_params);

	/** regularization constant \f$\eta > 0\f$ */
	T eta;

	/** regularizer params */
	param_type regularizer_params;
};

/** @brief class ForwardRegression for a dummy algorithm which does nothing and returns
 * all the features that are there in the input problem
 */
template <template <class> class Regularizer>
class ForwardRegression
{
public:
	/** parameter type */
	typedef ForwardRegressionParam<Regularizer, float64_t> param_type;

	/** constructor
	 * @param _regressors the regressors (real valued dense feature matrix)
	 * @param _regressand the regressand (real valued dense labels vector)
	 * @param _target_feats number of target features (default value is 0)
	 */
	ForwardRegression(const Matrix<float64_t>& _regressors, const Vector<float64_t>& _regressand,
			index_t _target_feats = 0);

	/** destructor */
	~ForwardRegression();

	/** @return the vector of selected feature indices */
	std::vector<index_t> run();

	/** @param param the parameter type */
	void set_params(param_type _params);

private:
	/** parameters */
	param_type params;

	/** real valued dense feature matrix */
	const Matrix<float64_t>& regressors;

	/** real valued dense labels vector */
	const Vector<float64_t>& regressand;

	/** number of target features */
	index_t target_feats;
};

}

#endif // FORWARD_REGRESSION_H__
