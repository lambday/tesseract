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

#ifndef GREEDY_LOCAL_SEARCH_H__
#define GREEDY_LOCAL_SEARCH_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief struct for parameters used in greedy local-search */
template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
struct GreedyLocalSearchParam
{
	/** forward regression algo param type */
	typedef typename FRAlgo<Regularizer,T>::param_type fr_param_type;

	/** local search algo param type */
	typedef typename LSAlgo<Regularizer,T>::param_type ls_param_type;

	/** regularizer param type */
	typedef typename Regularizer<T>::param_type reg_param_type;

	/** default constructor */
	GreedyLocalSearchParam();

	/** constructor */
	GreedyLocalSearchParam(ls_param_type ls_params);

	/** constructor */
	GreedyLocalSearchParam(T _eta, reg_param_type _regularizer_params);

	/** eta */
	T eta;

	/** regularizer params */
	reg_param_type regularizer_params;

	/** forward regression params */
	fr_param_type fr_params;

	/** local search params */
	ls_param_type ls_params;
};

/** @brief class GreedyLocalSearch for a dummy algorithm which does nothing and returns
 * all the features that are there in the input problem
 */
template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
class GreedyLocalSearch
{
public:
	/** parameter type */
	typedef GreedyLocalSearchParam<FRAlgo,LSAlgo,Regularizer,T> param_type;

	/** constructor
	 * @param _cov the covariance matrix
	 * @param _target_feats number of target features (default value is 0)
	 */
	GreedyLocalSearch(const Eigen::Ref<const Matrix<T>>& _cov,
			index_t _target_feats = 0);

	/** destructor */
	~GreedyLocalSearch();

	/** @return a pair of the function value and selected feature indices */
	std::pair<T,std::vector<index_t>> run();

	/** @param param the parameter type */
	void set_params(param_type _params);

private:
	/** parameters */
	param_type params;

	/** real valued dense covariance matrix */
	const Eigen::Ref<const Matrix<float64_t>> cov;

	/** number of target features */
	index_t target_feats;
};

}

#endif // GREEDY_LOCAL_SEARCH_H__
