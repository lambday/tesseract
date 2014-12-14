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
#include <tesseract/algorithm/LinearLocalSearch.hpp>
#include <tesseract/computation/ComputeFunction.hpp>
#include <tesseract/regularizer/DummyRegularizer.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <tesseract/regularizer/SpectralVariance.hpp>
#include <tesseract/features/Features.hpp>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

using namespace tesseract;

template <template <class> class Regularizer, typename T>
LinearLocalSearchParam<Regularizer, T>::LinearLocalSearchParam()
:eta(ComputeFunction<Regularizer,T>::default_eta)
{
}

template <template <class> class Regularizer, typename T>
LinearLocalSearchParam<Regularizer, T>::LinearLocalSearchParam(T _eta)
:eta(_eta)
{
}

template <template <class> class Regularizer, typename T>
LinearLocalSearchParam<Regularizer, T>::LinearLocalSearchParam(T _eta,reg_param_type reg_params)
:eta(_eta), regularizer_params(reg_params)
{
}

template <template <class> class Regularizer, typename T>
LinearLocalSearch<Regularizer, T>::LinearLocalSearch(const Eigen::Ref<const Matrix<T>>& _cov)
: cov(_cov), global_value(0)
{
}

template <template <class> class Regularizer, typename T>
LinearLocalSearch<Regularizer, T>::~LinearLocalSearch()
{
}

template <template <class> class Regularizer, typename T>
std::pair<T,std::vector<index_t>> LinearLocalSearch<Regularizer, T>::run()
{
	logger.write(Debug, "Entering %s!\n", __PRETTY_FUNCTION__);

	// number of total features
	index_t n = cov.cols() - 1;

	// create the regularizer function
	Regularizer<T> f;
	f.set_params(params.regularizer_params);

	// computing the initial values
	T f_Xi = 0;
	T f_Yi = f(cov.topLeftCorner(n, n));

	logger.write(Debug, "f(null) = %f, f(all) = %f\n", f_Xi, f_Yi);

	// make sure the function is non-negative
	assert(f_Yi >= 0.0);

	// index sets
	std::vector<index_t> inds;
	inds.reserve(n);

	std::vector<bool> removed(n);
	std::fill(removed.begin(), removed.end(), false);

	// flag for avoiding covariance matrix copying if indices are either added or
	// removed consecutively
	bool is_consecutive = true;

	// functor to compute if all of the features are being added to X or being
	// removed from Y to check whether the operation is consecutive
	std::function<bool(std::vector<bool>&, index_t i)> all_same =
		[](std::vector<bool>& v, index_t i)
	{
		return std::all_of(v.begin(), v.begin()+i+1, [](bool s){ return s; }) ||
			std::all_of(v.begin(), v.begin()+i+1, [](bool s){ return !s; });
	};

	// functor to compute unremoved indices from the removed list
	std::function<void(std::vector<bool>&, std::vector<index_t>&)> find_unremoved_inds =
		[](std::vector<bool>& removed, std::vector<index_t>& inds)
	{
		for (index_t i = 0; i < removed.size(); ++i)
		{
			// if the current index is not removed, put it in the Y list
			if (!removed[i]) inds.push_back(i);
		}
	};

	// main loop of linear time local search
	for (index_t i = 0; i < n; ++i)
	{
		// current function values
		// current_fX is f(X_{i+1}) which is f(X_{i-1}\cup \{x_i\})
		// current_fY is f(Y_{i+1}) which is f(Y_{i-1}\setminus \{x_i\})
		T current_fX = 0;
		T current_fY = 0;

		// if added or removed indices are consecutive, we don't need to copy the
		// covariance matrix. we can just specify a contiguous block of it and
		// save time required by copying
		if (is_consecutive)
		{
			logger.write(Debug, "i = %d, consecutive, no copy required!\n", i);

			// for consecutive case, submatrix of cov(0,0) to cov(i,i) will be c_S
			current_fX = f(cov.topLeftCorner(i+1,i+1));

			if (logger.get_loglevel() >= MemDebug)
			{
				logger.print_matrix(cov.topLeftCorner(i+1,i+1));
			}

			// similarly, submatrix of cov(i+1,i+1) to cov(n-1,n-1) will be c_(U-S)
			current_fY = f(cov.topLeftCorner(n,n).bottomRightCorner(n-i-1,n-i-1));

			if (logger.get_loglevel() >= MemDebug)
			{
				logger.print_matrix(cov.topLeftCorner(n,n).bottomRightCorner(n-i-1,n-i-1));
			}
		}
		else
		{
			// we have to copy the covariance matrix for the indices
			{
				std::vector<index_t> cur_inds(inds);
				cur_inds.push_back(i);

				Matrix<T> c_X = Features<T>::copy_cov(cov, cur_inds);
				current_fX = f(c_X);

				if (logger.get_loglevel() >= MemDebug)
				{
					logger.print_matrix(c_X);
				}
			}

			{
				std::vector<bool> cur_removed(removed);
				cur_removed[i] = true;
				std::vector<index_t> unremoved_inds;
				find_unremoved_inds(cur_removed, unremoved_inds);

				Matrix<T> c_Y = Features<T>::copy_cov(cov, unremoved_inds);
				current_fY = f(c_Y);

				if (logger.get_loglevel() >= MemDebug)
				{
					logger.print_matrix(c_Y);
				}
			}
		}

		logger.write(Debug,"%d: f(Xi) = %f, f(Yi) = %f\n", i, current_fX, current_fY);

		// compute the differences
		T a_i = current_fX - f_Xi;
		T b_i = current_fY - f_Yi;

		logger.write(Debug,"%d: a_i = %f, b_i = %f\n", i, a_i, b_i);

		// if both are zero the we should remove it
		if (a_i >= b_i && a_i > 0.0)
		{
			// add i-th element to X, keep Y the same
			inds.push_back(i);
			logger.write(Debug, "%d is added to Xn!\n", i);
		}
		else
		{
			// keep X same, remove i-th element from Y
			removed[i] = true;
			logger.write(Debug, "%d is removed from Yn!\n", i);
		}

		// check if addition or removal is still consecutive
		is_consecutive = all_same(removed, i);

		// update the function values
		f_Xi = current_fX;
		f_Yi = current_fY;

		// make sure that the function is non-negative
		assert(f_Xi >= 0.0 && f_Yi >= 0.0);
	}

	// make sure that Xn and Yn are indeed the same
	std::vector<index_t> Yn;
	find_unremoved_inds(removed, Yn);

	logger.write(Debug, "f(S) = %f\n", f_Xi);

	if (logger.get_loglevel() >= MemDebug)
	{
		logger.print_vector(inds);
		logger.print_vector(Yn);
	}

	logger.write(Debug, "selected features = %u!\n", inds.size());

	// the indices are added in sorted order already
	assert(inds.size() == Yn.size());
	for (index_t i = 0; i < inds.size(); ++i)
	{
		assert(inds[i] == Yn[i]);
	}

	// if none of the features were added or all of the features were added
	// then save some computation since we don't need to evaluate g at all
	// because LS was useless - returning 0 value instead
	if (is_consecutive)
	{
		return std::make_pair(0.0, inds);
	}

	// otherwise, compute the objective function and compare with U and U \ S
	ComputeFunction<Regularizer, T> g;
	g.set_eta(params.eta);
	g.set_reg_params(params.regularizer_params);

	// reference of the indices that will be returned
	std::vector<index_t>& ret_inds = inds;

	// make sure that the last row/col is also included since it contains the b_S part
	inds.push_back(n);

	// on S
	Matrix<T> c_s = Features<T>::copy_cov(cov, inds);
	T retval = g(c_s);

	// on U \ S and U
	std::vector<index_t> rest_inds;
	std::vector<index_t> all_inds;

	// if all features are selected, then no use of checking any further

	// compute the indices U \ S
	for (index_t i = 0; i < removed.size(); ++i)
	{
		if (removed[i])
		{
			rest_inds.push_back(i);
		}
	}

	// make sure that the last row/col is also included since it contains the b_S part
	rest_inds.push_back(n);
	c_s = Features<T>::copy_cov(cov, rest_inds);
	T g_UminusS = g(c_s);

	if (g_UminusS > retval)
	{
		ret_inds = rest_inds;
		retval = g_UminusS;
	}

	// last col already included
	all_inds.resize(n + 1);
	std::iota(all_inds.begin(), all_inds.end(), 0);

	// make use of computed global value if already provided externally
	T g_U = global_value;
	if (g_U == 0)
	{
		g_U = g(cov);
	}

	// compare finally
	if (g_U > retval)
	{
		ret_inds = all_inds;
		retval = g_U;
	}

	// make sure to pop back the last added index since it's the col related to Z
	ret_inds.pop_back();

	return std::make_pair(retval,ret_inds);
}

template <template <class> class Regularizer, typename T>
void LinearLocalSearch<Regularizer, T>::set_params(LinearLocalSearch<Regularizer, T>::param_type _params)
{
	params = _params;
}

template <template <class> class Regularizer, typename T>
void LinearLocalSearch<Regularizer, T>::set_global_value(T value)
{
	global_value = value;
}

template class LinearLocalSearchParam<DummyRegularizer, float64_t>;
template class LinearLocalSearchParam<SmoothedDifferentialEntropy, float64_t>;
template class LinearLocalSearchParam<SpectralVariance, float64_t>;
template class LinearLocalSearch<DummyRegularizer, float64_t>;
template class LinearLocalSearch<SmoothedDifferentialEntropy, float64_t>;
template class LinearLocalSearch<SpectralVariance, float64_t>;
