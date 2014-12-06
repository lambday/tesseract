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

template <template <class> class Regularizer>
LocalSearch<Regularizer>::LocalSearch(const Matrix<float64_t>& _regressors,
		const Vector<float64_t>& _regressand, float64_t _eps)
	: regressors(_regressors), regressand(_regressand), eps(_eps)
{
}

template <template <class> class Regularizer>
LocalSearch<Regularizer>::~LocalSearch()
{
}

template <template <class> class Regularizer>
std::vector<index_t> LocalSearch<Regularizer>::run()
{
	// number of total features
	index_t n = regressors.cols();

	// need to keep track of the maximum evaluated value of f
	float64_t maxval = 0;
	index_t argmax = -1;

	// create the regularizer function
	Regularizer<float64_t> f;

	// computing the argmax part
	for (index_t j = 0; j < n; ++j)
	{
		std::vector<index_t> inds;
		inds.push_back(j);

		// evaluate the function on the regressors
		const Matrix<float64_t>& m = Features<float64_t>::copy_feats(regressors, inds);
		float64_t val = f(m.transpose() * m);

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
	float64_t limit = 1 + eps / n / n;

	// if there exists any more features to increase f
	bool exists = false;

	// main loop
	do
	{
		// need to update since the last iteration
		exists = false;
		float64_t threshold = limit * maxval;

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
				const Matrix<float64_t>& m = Features<float64_t>::copy_feats(regressors, cur_inds);
				float64_t val = f(m.transpose() * m);

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
	ComputeFunction<Regularizer, float64_t> g;

	// reference of the indices that will be returned
	std::vector<index_t>& ret_inds = inds;

	// on S
	float64_t g_S = g(Features<float64_t>::copy_feats(regressors,regressand,inds));

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

		float64_t g_UminusS = g(Features<float64_t>::copy_feats(regressors,regressand,rest_inds));

		if (g_UminusS > g_S)
		{
			ret_inds = rest_inds;
		}

		all_inds.resize(n);
		std::iota(all_inds.begin(), all_inds.end(), 0);

		float64_t g_U = g(Features<float64_t>::copy_feats(regressors,regressand,all_inds));

		if (g_U > g_UminusS)
		{
			ret_inds = all_inds;
		}
	}

	return ret_inds;
}

template class LocalSearch<DummyRegularizer>;
template class LocalSearch<SmoothedDifferentialEntropy>;
