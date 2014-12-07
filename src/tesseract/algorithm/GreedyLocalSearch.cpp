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

#include <tesseract/algorithm/GreedyLocalSearch.hpp>
#include <tesseract/algorithm/ForwardRegression.hpp>
#include <tesseract/algorithm/LocalSearch.hpp>
#include <tesseract/computation/ComputeFunction.hpp>
#include <tesseract/regularizer/DummyRegularizer.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <tesseract/features/Features.hpp>
#include <vector>
#include <functional>
#include <algorithm>

using namespace tesseract;

template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
GreedyLocalSearchParam<FRAlgo,LSAlgo,Regularizer,T>::GreedyLocalSearchParam()
{
	eta = ComputeFunction<Regularizer,T>::default_eta;
	regularizer_params = reg_param_type();
	fr_params = fr_param_type(eta,regularizer_params);
	ls_params = ls_param_type(eta,regularizer_params);
}

template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
GreedyLocalSearchParam<FRAlgo,LSAlgo,Regularizer,T>::GreedyLocalSearchParam(ls_param_type _ls_params)
{
	eta = _ls_params.eta;
	regularizer_params = _ls_params.regularizer_params;
	ls_params = _ls_params;
	fr_params = fr_param_type(eta,regularizer_params);
}

template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
GreedyLocalSearchParam<FRAlgo,LSAlgo,Regularizer,T>::GreedyLocalSearchParam(T _eta,
		reg_param_type _regularizer_params)
{
	eta = _eta;
	regularizer_params = _regularizer_params;
	fr_params = fr_param_type(eta,regularizer_params);
	ls_params = ls_param_type(eta,regularizer_params);
}

template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
GreedyLocalSearch<FRAlgo,LSAlgo,Regularizer,T>::GreedyLocalSearch(const Matrix<T>& _regressors,
		const Vector<T>& _regressand, index_t _target_feats)
	: regressors(_regressors), regressand(_regressand), target_feats(_target_feats)
{
}

template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
GreedyLocalSearch<FRAlgo,LSAlgo,Regularizer,T>::~GreedyLocalSearch()
{
}

template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
std::vector<index_t> GreedyLocalSearch<FRAlgo,LSAlgo,Regularizer,T>::run()
{
	// run forward regression on the whole data
	FRAlgo<Regularizer,T> fr(regressors, regressand, target_feats);
	fr.set_params(params.fr_params);
	std::vector<index_t> S_1 = fr.run();

	// sort the indices, needed for index mapping
	std::sort(S_1.begin(), S_1.end());

	// index map functor - needs unit-testing
	std::function<void(const std::vector<index_t>&,std::vector<index_t>&)> inds_map
		= [](const std::vector<index_t>& orig,std::vector<index_t>& relative)
		{
			std::for_each(relative.begin(), relative.end(), [orig](index_t& val)
			{
				val = orig[val];
			});
		};

	// run local search on the returned set
	LSAlgo<Regularizer,T> ls(Features<T>::copy_feats(regressors,S_1), regressand);
	ls.set_params(params.ls_params);
	std::vector<index_t> S_p = ls.run();

	// if returned set is of same size as of S_1, don't bother
	// otherwise, since the above returned indices are mapped, we need to map it back
	bool ls_useful = S_p.size() < S_1.size();
	if (ls_useful)
	{
		std::sort(S_p.begin(), S_p.end());
		inds_map(S_1, S_p);
	}

	// compute the rest of indices
	std::vector<index_t> rest;
	index_t in_s1 = 0;
	for (index_t i = 0; i < regressors.cols(); ++i)
	{
		if (S_1[in_s1] == i)
			in_s1++;
		else
			rest.push_back(i);
	}

	assert(S_1.size() + rest.size() == regressors.cols());

	// run forward regression for the rest of the data
	FRAlgo<Regularizer,T> fr2(Features<T>::copy_feats(regressors,rest), regressand, target_feats);
	fr2.set_params(params.fr_params);
	std::vector<index_t> S_2 = fr2.run();

	// map these indices
	std::sort(S_2.begin(), S_2.end());
	inds_map(rest, S_2);

	// create the compute function
	ComputeFunction<Regularizer, T> g;
	g.set_eta(params.eta);
	g.set_reg_params(params.regularizer_params);

	// return inds
	std::vector<index_t> inds = S_1;

	// compute g(S_1)
	T max = g(Features<T>::copy_feats(regressors,regressand,S_1));

	// compute g(S_p)
	if (ls_useful)
	{
		T g_S_p = g(Features<T>::copy_feats(regressors,regressand,S_p));
		if (max < g_S_p)
		{
			max = g_S_p;
			inds = S_p;
		}
	}

	// compute g(S_2)
	T g_S_2 = g(Features<T>::copy_feats(regressors,regressand,S_2));

	if (max < g_S_2)
	{
		max = g_S_2;
		inds = S_2;
	}

	return inds;
}

template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
void GreedyLocalSearch<FRAlgo,LSAlgo,Regularizer,T>::set_params(param_type _params)
{
	params = _params;
}

template class GreedyLocalSearchParam<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>;
template class GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>;
