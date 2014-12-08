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

#ifndef LOCAL_SEARCH_H__
#define LOCAL_SEARCH_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief class LocalSearchParam for storing local search parameters */
template <template <class> class Regularizer, typename T>
struct LocalSearchParam
{
	/** regularizer param type */
	typedef typename Regularizer<T>::param_type reg_param_type;

	/** default constructor */
	LocalSearchParam();

	/** constructor */
	LocalSearchParam(T _eta);

	/** constructor */
	LocalSearchParam(T _eta, reg_param_type reg_params);

	/** constructor */
	LocalSearchParam(T _eta, T _eps, reg_param_type reg_params);

	/** regularization constant \f$\eta > 0\f$ */
	T eta;

	/** epsilon of the algorithm */
	T eps;

	/** regularizer params */
	reg_param_type regularizer_params;

	/** default epsilon value */
	static constexpr T default_eps = static_cast<T>(22);
};

/** @brief class LocalSearch for a dummy algorithm which does nothing and returns
 * all the features that are there in the input problem
 */
template <template <class> class Regularizer, typename T>
class LocalSearch
{
public:
	/** parameter type */
	typedef LocalSearchParam<Regularizer, T> param_type;

	/** constructor
	 * @param _cov the covariance matrix
	 * @param _target_feats number of target features (default value is 0)
	 */
	LocalSearch(const Eigen::Ref<const Matrix<T>>& _cov);

	/** destructor */
	~LocalSearch();

	/** @return a pair of the function value and selected feature indices */
	std::pair<T,std::vector<index_t>> run();

	/** @param params the parameters of the algorithm */
	void set_params(param_type _params);

private:
	/** real valued dense covariance matrix */
	const Eigen::Ref<const Matrix<float64_t>> cov;

	/** the algorithm params */
	param_type params;
};

}

#endif // LOCAL_SEARCH_H__
