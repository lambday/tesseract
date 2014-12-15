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

#include <tesseract/base/init.hpp>
#include <tesseract/algorithm/GreedyLocalSearch.hpp>
#include <tesseract/algorithm/ForwardRegression.hpp>
#include <tesseract/algorithm/LocalSearch.hpp>
#include <tesseract/algorithm/LinearLocalSearch.hpp>
#include <tesseract/computation/ComputeFunction.hpp>
#include <tesseract/regularizer/DummyRegularizer.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <tesseract/regularizer/SpectralVariance.hpp>
#include <tesseract/features/Features.hpp>
#include <vector>
#include <functional>
#include <algorithm>
#include <chrono>

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
GreedyLocalSearch<FRAlgo,LSAlgo,Regularizer,T>::GreedyLocalSearch(const Eigen::Ref<const Matrix<T>>& _cov,
		index_t _target_feats)
: cov(_cov), target_feats(_target_feats)
{
	logger.write(MemDebug, "In %s\n", __PRETTY_FUNCTION__);
	logger.write(MemDebug, "cov.data = %p!\n", cov.data());
	logger.write(MemDebug, "cov.rows = %u!\n", cov.rows());
	logger.write(MemDebug, "cov.cols = %u!\n", cov.cols());
	logger.write(MemDebug, "target features = %u!\n", target_feats);
}

template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
GreedyLocalSearch<FRAlgo,LSAlgo,Regularizer,T>::~GreedyLocalSearch()
{
	logger.write(MemDebug, "%s Dying\n", __PRETTY_FUNCTION__);
}

template <template<template<class>class,typename> class FRAlgo,
		 template <template<class>class,typename> class LSAlgo,
		 template <class> class Regularizer, typename T>
std::pair<T,std::vector<index_t>> GreedyLocalSearch<FRAlgo,LSAlgo,Regularizer,T>::run()
{
	logger.write(Debug, "%s Entering!\n", __PRETTY_FUNCTION__);
	logger.write(MemDebug, "cov.data = %p!\n", cov.data());
	logger.write(MemDebug, "cov.rows = %u!\n", cov.rows());
	logger.write(MemDebug, "cov.cols = %u!\n", cov.cols());
	logger.write(MemDebug, "target features = %u!\n", target_feats);

	// total number of feats
	index_t N = cov.cols() - 1;
	logger.write(Debug, "total feats = %u!\n", N);

	// run forward regression on the whole data
	FRAlgo<Regularizer,T> fr(cov, target_feats);
	fr.set_params(params.fr_params);
	std::pair<T,std::vector<index_t>> S_1 = fr.run();
	T g_S_1 = S_1.first;
	std::vector<index_t> S_1_inds = S_1.second;

	// sort the indices, needed for index mapping
	std::sort(S_1_inds.begin(), S_1_inds.end());

	logger.write(Debug, "FR(S) = %f!\n", g_S_1);
	if (logger.get_loglevel() >= Debug)
	{
		logger.print_vector(S_1_inds);
	}

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
	// make sure to include the last column
	S_1_inds.push_back(N);

	logger.write(Debug, "For LS, copying cov with indices\n");
	if (logger.get_loglevel() >= Debug)
	{
		logger.print_vector(S_1_inds);
	}

	Matrix<T> cov_S_1 = Features<T>::copy_cov(cov, S_1_inds);
	LSAlgo<Regularizer,T> ls(cov_S_1);
	ls.set_params(params.ls_params);

	// make use of the function value we just computed using FR
	ls.set_global_value(g_S_1);

	// track the time taken by the LS algorithm

	// set timers
	const auto cpu0 = std::chrono::high_resolution_clock::now();

	std::pair<T,std::vector<index_t>> S_p = ls.run();

	// stop timers
	const auto cpu1 = std::chrono::high_resolution_clock::now();
	const auto cpu_elapsed = cpu1 - cpu0;

	logger.write(Special, "%.10f ", std::chrono::duration<float64_t>(cpu_elapsed).count());
	T g_S_p = S_p.first;

	logger.write(Special, "%.10f ", g_S_p);

	std::vector<index_t> S_p_inds = S_p.second;

	logger.write(Debug, "LS(S_1) = %f!\n", g_S_p);
	logger.write(Debug, "Relative indices returned by LS\n");
	if (logger.get_loglevel() >= Debug)
	{
		logger.print_vector(S_p_inds);
	}

	// pop back the last column from the index set
	S_1_inds.pop_back();

	// if returned set is of same size as of S_1, don't bother
	// otherwise, since the above returned indices are mapped, we need to map it back
	bool ls_useful = S_p_inds.size() > 0 && S_p_inds.size() < S_1_inds.size();
	if (ls_useful)
	{
		// whether LS was useful
		logger.write(Special, "+ ");

		std::sort(S_p_inds.begin(), S_p_inds.end());
		inds_map(S_1_inds, S_p_inds);

		logger.write(Debug, "Actual indices returned by LS\n");
		if (logger.get_loglevel() >= Debug)
		{
			logger.print_vector(S_p_inds);
		}
	}
	else
	{
		// whether LS was useful
		logger.write(Special, "- ");
	}

	// compute the rest of indices
	std::vector<index_t> rest;
	index_t in_s1 = 0;
	for (index_t i = 0; i < N; ++i)
	{
		if (S_1_inds[in_s1] == i)
			in_s1++;
		else
			rest.push_back(i);
	}

	assert(S_1_inds.size() + rest.size() == N);

	logger.write(Debug, "Rest of the indices than returned by FR\n");
	if (logger.get_loglevel() >= Debug)
	{
		logger.print_vector(rest);
	}

	// make sure to add the last column
	rest.push_back(N);

	logger.write(Debug, "For FR2, copying cov with indices\n");
	if (logger.get_loglevel() >= Debug)
	{
		logger.print_vector(rest);
	}

	// run forward regression for the rest of the data
	Matrix<T> cov_rest = Features<T>::copy_cov(cov, rest);
	FRAlgo<Regularizer,T> fr2(cov_rest,target_feats);
	fr2.set_params(params.fr_params);

	std::pair<T,std::vector<index_t>> S_2 = fr2.run();
	T g_S_2 = S_2.first;
	std::vector<index_t> S_2_inds = S_2.second;

	// pop back last column index from rest
	rest.pop_back();

	logger.write(Debug, "FR(U-S_1) = %f!\n", g_S_2);
	logger.write(Debug, "Relative indices returned by FR\n");
	if (logger.get_loglevel() >= Debug)
	{
		logger.print_vector(S_2_inds);
	}

	// map these indices
	std::sort(S_2_inds.begin(), S_2_inds.end());
	inds_map(rest, S_2_inds);

	logger.write(Debug, "Actual indices returned by FR\n");
	if (logger.get_loglevel() >= Debug)
	{
		logger.print_vector(S_2_inds);
	}

	// return inds
	std::vector<index_t>& argmax = S_1_inds;
	T maxval = g_S_1;

	enum Selected { FR1, LS, FR2 };
	Selected selected = FR1;

	if (maxval < g_S_p)
	{
		selected = LS;
		maxval = g_S_p;
		argmax = S_p_inds;
	}

	if (maxval < g_S_2)
	{
		selected = FR2;
		maxval = g_S_2;
		argmax = S_2_inds;
	}

	// special debug messages
	// - 0 for FR1 was selected
	// - 1 for LS was selected
	// - 2 for FR2 was selected
	logger.write(Special, "%d ", selected);

	logger.write(Debug, "%s Exiting!\n", __PRETTY_FUNCTION__);
	return std::make_pair(maxval, argmax);
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

template class GreedyLocalSearchParam<ForwardRegression,LinearLocalSearch,SmoothedDifferentialEntropy,float64_t>;
template class GreedyLocalSearch<ForwardRegression,LinearLocalSearch,SmoothedDifferentialEntropy,float64_t>;

template class GreedyLocalSearchParam<ForwardRegression,LocalSearch,SpectralVariance,float64_t>;
template class GreedyLocalSearch<ForwardRegression,LocalSearch,SpectralVariance,float64_t>;

template class GreedyLocalSearchParam<ForwardRegression,LinearLocalSearch,SpectralVariance,float64_t>;
template class GreedyLocalSearch<ForwardRegression,LinearLocalSearch,SpectralVariance,float64_t>;
