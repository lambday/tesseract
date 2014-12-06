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

#ifndef EVALUATION_H__
#define EVALUATION_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief template class Evaluation which performs the whole training and testing
 * process via evaluate() method.
 */
template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
class Evaluation
{
public:
	/**
	 * performs training on training dataset as specified
	 * @return the indices of the selected features
	 */
	std::vector<index_t> train();

	/**
	 * @param indices the indices of the selected features returned by the train() method
	 * @return a pair containing number of features used and error measure based
	 */
	std::pair<index_t, float64_t> test(std::vector<index_t> indices);

	/** @return a pair containing number of features used and error measure based */
	std::pair<index_t, float64_t> evaluate();

	/** @param _param params to be used by the algorithm */
	void set_params(typename Algorithm::param_type _params);

	/** @param _seed seed to be used for data generation */
	void set_seed(int32_t _seed);

	/** @param _num_examples number of examples to be used for the evaluation */
	void set_num_examples(index_t _num_examples);

	/** @param _target_feats the number of desired features for the feature selection
	 * algorithm. 0 value indicates that all the features will be used
	 */
	void set_target_feats(index_t _target_feats);

private:
	/** parameters for the training algorithm */
	typename Algorithm::param_type params;

	/** seed for random number generator for data generation */
	int32_t seed;

	/** number of examples to be used for current evaluation */
	index_t num_examples;

	/** number of desired features for the feature selection algorithm */
	index_t target_feats;
};

}

#endif // EVALUATION_H__
