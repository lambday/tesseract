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
#include <tesseract/algorithm/ForwardRegression.hpp>
#include <tesseract/computation/ComputeFunction.hpp>
#include <tesseract/regularizer/DummyRegularizer.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <tesseract/regularizer/SpectralVariance.hpp>
#include <tesseract/features/Features.hpp>
#include <vector>
#include <map>

using namespace tesseract;

template <template <class> class Regularizer, typename T>
ForwardRegressionParam<Regularizer, T>::ForwardRegressionParam()
: eta(ComputeFunction<Regularizer,T>::default_eta)
{
}

template <template <class> class Regularizer, typename T>
ForwardRegressionParam<Regularizer, T>::ForwardRegressionParam(T _eta,
		ForwardRegressionParam<Regularizer,T>::reg_param_type reg_params)
: eta(_eta), regularizer_params(reg_params)
{
}

template <template <class> class Regularizer, typename T>
ForwardRegression<Regularizer,T>::ForwardRegression(const Eigen::Ref<const Matrix<T>>& _cov,
		index_t _target_feats)
: cov(_cov), target_feats(_target_feats)
{
	logger.write(MemDebug, "In %s\n", __PRETTY_FUNCTION__);
	logger.write(MemDebug, "cov.data = %p!\n", cov.data());
	logger.write(MemDebug, "cov.rows = %u!\n", cov.rows());
	logger.write(MemDebug, "cov.cols = %u!\n", cov.cols());
	logger.write(MemDebug, "target features = %u!\n", target_feats);
}

template <template <class> class Regularizer, typename T>
ForwardRegression<Regularizer,T>::~ForwardRegression()
{
	logger.write(MemDebug, "%s Dying\n", __PRETTY_FUNCTION__);
}

template <template <class> class Regularizer, typename T>
std::pair<T,std::vector<index_t>> ForwardRegression<Regularizer,T>::run()
{
	logger.write(Debug, "%s Entering!\n", __PRETTY_FUNCTION__);
	logger.write(MemDebug, "cov.data = %p!\n", cov.data());
	logger.write(MemDebug, "cov.rows = %u!\n", cov.rows());
	logger.write(MemDebug, "cov.cols = %u!\n", cov.cols());
	logger.write(MemDebug, "target features = %u!\n", target_feats);

	// number of feats
	index_t N = cov.cols() - 1;
	logger.write(Debug, "total feats = %u!\n", N);

	if (logger.get_loglevel() >= MemDebug)
	{
		logger.print_matrix(cov);
	}

	// create the compute function
	ComputeFunction<Regularizer, T> g;
	g.set_eta(params.eta);
	g.set_reg_params(params.regularizer_params);

	// return vector - indices of selected features
	std::vector<index_t> inds;

	// status vector to avoid overchecking
	std::vector<bool> selected(N);
	std::fill(selected.begin(), selected.end(), false);

	// final function value
	T maxval = 0;

	// main loop runs until target_feats features are added
	// cannot be parallelised since addition happens serially
	for (index_t i = 0; i < target_feats; ++i)
	{
		// store the values for argmax operation
		maxval = 0;
		index_t argmax = -1;

		// can be parallelised
		for (index_t j = 0; j < N; ++j)
		{
			if (!selected[j])
			{
				// note that in case of parallel, inds should have individual copies
				// the following indices are for C_S
				std::vector<index_t> cur_inds(inds);
				cur_inds.push_back(j);

				// remember to push the index of last column
				// since we need b_S also for compute function
				cur_inds.push_back(N);

				// evaluate the function
				Matrix<float64_t> c_s = Features<T>::copy_cov(cov, cur_inds);

				logger.write(MemDebug, "for indices\n");
				if (logger.get_loglevel() >= MemDebug)
				{
					logger.print_vector(cur_inds);
				}
				logger.write(MemDebug, "cov.data in use = %p!\n", c_s.data());
				if (logger.get_loglevel() >= MemDebug)
				{
					logger.print_matrix(c_s);
				}

				T val = g(c_s);
				logger.write(Debug, "j = %u, val = %f, maxval = %f, argmax = %u!\n",
						j, val, maxval, argmax);

				// update running max, need to be write protected
				if (val > maxval)
				{
					maxval = val;
					argmax = j;
				}
			}
		}

		logger.write(Debug, "i = %u, maxval = %f, argmax = %u!\n", i, maxval, argmax);

		// make sure that we added something
		assert(argmax != -1);

		// update the working set
		inds.push_back(argmax);
		selected[argmax] = true;
	}

	return std::make_pair(maxval, inds);
}

template <template <class> class Regularizer, typename T>
void ForwardRegression<Regularizer,T>::set_params(ForwardRegression<Regularizer,T>::param_type _params)
{
	params = _params;
}

template class ForwardRegressionParam<DummyRegularizer, float64_t>;
template class ForwardRegressionParam<SmoothedDifferentialEntropy, float64_t>;
template class ForwardRegressionParam<SpectralVariance, float64_t>;
template class ForwardRegression<DummyRegularizer, float64_t>;
template class ForwardRegression<SmoothedDifferentialEntropy, float64_t>;
template class ForwardRegression<SpectralVariance, float64_t>;
