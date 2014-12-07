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

#include <tesseract/algorithm/LocalSearch.hpp>
#include <tesseract/computation/ComputeFunction.hpp>
#include <tesseract/regularizer/DummyRegularizer.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <tesseract/features/Features.hpp>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

using namespace tesseract;

template <template <class> class Regularizer, typename T>
LocalSearchParam<Regularizer, T>::LocalSearchParam()
:eta(ComputeFunction<Regularizer,T>::default_eta),
	eps(LocalSearchParam<Regularizer,T>::default_eps)
{
}

template <template <class> class Regularizer, typename T>
LocalSearchParam<Regularizer, T>::LocalSearchParam(T _eta)
:eta(_eta), eps(LocalSearchParam<Regularizer,T>::default_eps)
{
}

template <template <class> class Regularizer, typename T>
LocalSearchParam<Regularizer, T>::LocalSearchParam(T _eta,reg_param_type reg_params)
:eta(_eta), eps(LocalSearchParam<Regularizer,T>::default_eps),
	regularizer_params(reg_params)
{
}

template <template <class> class Regularizer, typename T>
LocalSearchParam<Regularizer, T>::LocalSearchParam(T _eta, T _eps,
		LocalSearchParam<Regularizer,T>::reg_param_type reg_params)
: eta(_eta), eps(_eps), regularizer_params(reg_params)
{
}

template <template <class> class Regularizer, typename T>
LocalSearch<Regularizer, T>::LocalSearch(const Matrix<T>& _regressors,
		const Vector<T>& _regressand)
	: regressors(_regressors), regressand(_regressand)
{
}

template <template <class> class Regularizer, typename T>
LocalSearch<Regularizer, T>::~LocalSearch()
{
}

template <template <class> class Regularizer, typename T>
std::vector<index_t> LocalSearch<Regularizer, T>::run()
{
	// number of total features
	index_t n = regressors.cols();

	// need to keep track of the maximum evaluated value of f
	T maxval = 0;
	index_t argmax = -1;

	// create the regularizer function
	Regularizer<T> f;
	f.set_params(params.regularizer_params);

	// computing the argmax part
	for (index_t j = 0; j < n; ++j)
	{
		std::vector<index_t> inds;
		inds.push_back(j);

		// evaluate the function on the regressors
		const Matrix<T>& m = Features<T>::copy_feats(regressors, inds);
		T val = f(m.transpose() * m);

		// update running max and argmax
		if (val > maxval)
		{
			maxval = val;
			argmax = j;
		}
	}

	if (argmax == -1)
	{
		std::cerr << "regularizer must have to be positive submodular monotone" << std::endl;
		exit (3);
	}

	// return vector - indices of selected features
	std::vector<index_t> inds;

	// status vector to avoid overchecking
	std::vector<bool> selected(n);
	std::fill(selected.begin(), selected.end(), false);

	// update the working set
	inds.push_back(argmax);
	selected[argmax] = true;

	// fancy way of handling the end condition of the main loop
	std::function<bool(bool)> end_cond = [n, inds](bool exists)
	{
		// returns true if (a) all the features are added or
		// (b) there are no more feature that can increase f
		return inds.size() < n && exists;
	};

	// limit
	T limit = 1 + params.eps / n / n;

	// if there exists any more features to increase f
	bool exists = false;

	// main loop
	do
	{
		// need to update since the last iteration
		exists = false;
		T threshold = limit * maxval;

		// loop through the features
		for (index_t j = 0; j < n && !exists; ++j)
		{
			// make sure to check only those that are not already added
			if (!selected[j])
			{
				// need a local copy of the indices
				std::vector<index_t> cur_inds(inds);
				cur_inds.push_back(j);

				// evaluate the function on the regressors
				const Matrix<T>& m = Features<T>::copy_feats(regressors, cur_inds);
				T val = f(m.transpose() * m);

				// update running max and argmax
				if (exists = val >= threshold)
				{
					maxval = val;
					argmax = j;
				}
			}
		}

		// update the working set
		if (exists)
		{
			inds.push_back(argmax);
			selected[argmax] = true;
		}

	} while (end_cond(exists));

	// finally, compute the objective function
	ComputeFunction<Regularizer, T> g;
	g.set_eta(params.eta);
	g.set_reg_params(params.regularizer_params);

	// reference of the indices that will be returned
	std::vector<index_t>& ret_inds = inds;

	// on S
	T g_S = g(Features<T>::copy_feats(regressors,regressand,inds));

	// on U\S and U
	std::vector<index_t> rest_inds;
	std::vector<index_t> all_inds;

	// if all features are selected, then no use of checking any further
	if (inds.size() < n)
	{
		for (index_t i = 0; i < selected.size(); ++i)
		{
			if (!selected[i])
			{
				rest_inds.push_back(i);
			}
		}

		T g_UminusS = g(Features<T>::copy_feats(regressors,regressand,rest_inds));

		if (g_UminusS > g_S)
		{
			ret_inds = rest_inds;
		}

		all_inds.resize(n);
		std::iota(all_inds.begin(), all_inds.end(), 0);

		T g_U = g(Features<T>::copy_feats(regressors,regressand,all_inds));

		if (g_U > g_UminusS)
		{
			ret_inds = all_inds;
		}
	}

	return ret_inds;
}

template <template <class> class Regularizer, typename T>
void LocalSearch<Regularizer, T>::set_params(LocalSearch<Regularizer, T>::param_type _params)
{
	params = _params;
}

template class LocalSearchParam<DummyRegularizer, float64_t>;
template class LocalSearchParam<SmoothedDifferentialEntropy, float64_t>;
template class LocalSearch<DummyRegularizer, float64_t>;
template class LocalSearch<SmoothedDifferentialEntropy, float64_t>;
