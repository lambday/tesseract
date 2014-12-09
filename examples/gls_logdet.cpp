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

#include <tesseract/io/IDX1Reader.hpp>
#include <tesseract/io/IDX3Reader.hpp>
#include <tesseract/preprocessor/DataGenerator.hpp>
#include <tesseract/normalizer/UnitL2Normalizer.hpp>
#include <tesseract/algorithm/Dummy.hpp>
#include <tesseract/algorithm/ForwardRegression.hpp>
#include <tesseract/algorithm/LocalSearch.hpp>
#include <tesseract/algorithm/GreedyLocalSearch.hpp>
#include <tesseract/errors/SumSquaredError.hpp>
#include <tesseract/errors/PearsonsCorrelation.hpp>
#include <tesseract/errors/SquaredMultipleCorrelation.hpp>
#include <tesseract/evaluation/Evaluation.hpp>
#include <tesseract/evaluation/DataSet.hpp>
#include <tesseract/regularizer/DummyRegularizer.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <ctime>
#include <map>
#include <iostream>

using namespace tesseract;
/** this examples shows how to use existing evaluation framework to measure error/correlation */
template <class ErrorMeasures>
void test(float64_t eta, float64_t eps, float64_t delta, int seed)
{
	index_t num_examples = 1000;

	Evaluation<
		MNISTDataSet, // data set
		DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>, // data generator
		GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>, // algorithm
		ErrorMeasures // error measure
			> evaluator;

	evaluator.set_seed(seed);
	evaluator.set_num_examples(num_examples);

	// set algorithm params
	typedef GreedyLocalSearch<ForwardRegression,LocalSearch,
			SmoothedDifferentialEntropy,float64_t>::param_type param_type;

	typedef SmoothedDifferentialEntropy<float64_t>::param_type reg_param_type;
	reg_param_type reg_params(delta);

	param_type params(LocalSearchParam<SmoothedDifferentialEntropy,float64_t>(eta,eps,reg_params));
	evaluator.set_params(params);

	// run for different number of target feats
	for (index_t target_feats = 10; target_feats < 90; ++target_feats)
	{
		evaluator.set_target_feats(target_feats);
		std::pair<index_t,float64_t> result = evaluator.evaluate();

		// print two columns
		std::cout << result.first << "\t" << result.second << std::endl;
	}
}

int main(int argc, char** argv)
{
	float64_t eta = 1.0;
	float64_t eps = 1.0;
	float64_t delta = 0.1;
	int seed = 100;

	std::cout << "eta = " << eta << ", eps = " << eps << ", delta = " << delta << std::endl;
	test<SquaredMultipleCorrelation<float64_t>>(eta, eps, delta, seed);

	return 0;
}
