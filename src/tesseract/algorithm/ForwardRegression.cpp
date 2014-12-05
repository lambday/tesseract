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

#include <tesseract/algorithm/ForwardRegression.hpp>
#include <tesseract/computation/ComputeFunction.hpp>
#include <tesseract/regularizer/DummyRegularizer.hpp>
#include <tesseract/features/Features.hpp>
#include <vector>

using namespace tesseract;

template <template <class> class Regularizer>
ForwardRegression<Regularizer>::ForwardRegression(const Matrix<float64_t>& _regressors,
		const Vector<float64_t>& _regressand, index_t _target_feats)
	: regressors(_regressors), regressand(_regressand), target_feats(_target_feats)
{
}

template <template <class> class Regularizer>
ForwardRegression<Regularizer>::~ForwardRegression()
{
}

template <template <class> class Regularizer>
std::vector<index_t> ForwardRegression<Regularizer>::run()
{
	// create the compute function
	ComputeFunction<Regularizer, float64_t> g;

	// return vector - indices of selected features
	std::vector<index_t> inds;

	// status vector to avoid overchecking
	std::vector<bool> selected(regressors.cols());
	std::fill(selected.begin(), selected.end(), false);

	// main loop runs until target_feats features are added
	// cannot be parallelised since addition happens serially
	for (index_t i = 0; i < target_feats; ++i)
	{
		// store the values for argmax operation
		float64_t max = 0;
		index_t max_inds = -1;

		// can be parallelised
		for (index_t j = 0; j < regressors.cols(); ++j)
		{
			if (!selected[j])
			{
				// note that in case of parallel, inds should have individual copies
				std::vector<index_t> cur_inds(inds);
				cur_inds.push_back(j);

				// evaluate the function
				const Matrix<float64_t>& m = Features<float64_t>::copy_feats(regressors, regressand, cur_inds);
				float64_t val = g(m);

				// update running max, need to be write protected
				if (val > max)
				{
					max = val;
					max_inds = j;
				}

			}
		}

		// update selected indices
		inds.push_back(max_inds);

		// update selected flag
		selected[max_inds] = true;
	}

	return inds;
}

template class ForwardRegression<DummyRegularizer>;
