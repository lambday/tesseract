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
#include <tesseract/io/IDX1Reader.hpp>
#include <tesseract/io/IDX3Reader.hpp>
#include <tesseract/preprocessor/DataGenerator.hpp>
#include <tesseract/normalizer/UnitL2Normalizer.hpp>
#include <tesseract/algorithm/Dummy.hpp>
#include <tesseract/algorithm/ForwardRegression.hpp>
#include <tesseract/algorithm/LocalSearch.hpp>
#include <tesseract/algorithm/LinearLocalSearch.hpp>
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

template <class Algorithm, class ErrorMeasures>
void test(index_t num_examples, index_t target_feats)
{
	Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		Algorithm,ErrorMeasures> evaluator;
	//evaluator.set_seed(std::time(0));
	evaluator.set_seed(12345);
	evaluator.set_num_examples(num_examples);
	evaluator.set_target_feats(target_feats);

	// run with default set of params
	evaluator.set_params(typename Algorithm::param_type());

	std::pair<index_t,float64_t> result = evaluator.evaluate();

	std::cout << "number of datapoints " << num_examples << std::endl;
	std::cout << "number of features used " << result.first << std::endl;
	std::cout << "statistic measure " << result.second << std::endl;
}

int main(int argc, char** argv)
{
	index_t num_examples = 1000;
	index_t target_feats = 20;

	logger.set_loglevel(None);

	std::cout << "Test 1 : Dummy algorithm, Sum-squared error" << std::endl;
	std::cout << "===========================================" << std::endl;
	test<Dummy,SumSquaredError<float64_t>>(num_examples, target_feats);
	std::cout << "Test 2 : Dummy algorithm, Pearson's correlation" << std::endl;
	std::cout << "===============================================" << std::endl;
	test<Dummy,PearsonsCorrelation<float64_t>>(num_examples, target_feats);
	std::cout << "Test 3 : Dummy algorithm, Squared multiple correlation" << std::endl;
	std::cout << "======================================================" << std::endl;
	test<Dummy,SquaredMultipleCorrelation<float64_t>>(num_examples, target_feats);

	std::cout << "Test 4 : Forward regression, dummy regularizer, Sum-squared error" << std::endl;
	std::cout << "=================================================================" << std::endl;
	test<ForwardRegression<DummyRegularizer,float64_t>,
		SumSquaredError<float64_t>>(num_examples, target_feats);
	std::cout << "Test 5 : Forward regression, dummy regularizer, Pearson's correlation" << std::endl;
	std::cout << "=====================================================================" << std::endl;
	test<ForwardRegression<DummyRegularizer,float64_t>,
		PearsonsCorrelation<float64_t>>(num_examples, target_feats);
	std::cout << "Test 6 : Forward regression, dummy regularizer, Squared multiple correlation" << std::endl;
	std::cout << "============================================================================" << std::endl;
	test<ForwardRegression<DummyRegularizer,float64_t>,
		SquaredMultipleCorrelation<float64_t>>(num_examples, target_feats);

	std::cout << "Test 7 : Forward regression, logdet regularizer, Sum-squared error" << std::endl;
	std::cout << "=================================================================" << std::endl;
	test<ForwardRegression<SmoothedDifferentialEntropy,float64_t>,
		SumSquaredError<float64_t>>(num_examples, target_feats);
	std::cout << "Test 8 : Forward regression, logdet regularizer, Pearson's correlation" << std::endl;
	std::cout << "=====================================================================" << std::endl;
	test<ForwardRegression<SmoothedDifferentialEntropy,float64_t>,
		PearsonsCorrelation<float64_t>>(num_examples, target_feats);
	std::cout << "Test 9 : Forward regression, logdet regularizer, Squared multiple correlation" << std::endl;
	std::cout << "============================================================================" << std::endl;
	test<ForwardRegression<SmoothedDifferentialEntropy,float64_t>,
		SquaredMultipleCorrelation<float64_t>>(num_examples, target_feats);

	std::cout << "Test 10: GLS(FR, LS, logdet regularizer) Sum-squared error" << std::endl;
	std::cout << "=================================================================" << std::endl;
	test<GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>,
		SumSquaredError<float64_t>>(num_examples, target_feats);
	std::cout << "Test 11: GLS(FR, LS, logdet regularizer) Pearson's correlation" << std::endl;
	std::cout << "=====================================================================" << std::endl;
	test<GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>,
		PearsonsCorrelation<float64_t>>(num_examples, target_feats);
	std::cout << "Test 12: GLS(FR, LS, logdet regularizer) Squared multiple correlation" << std::endl;
	std::cout << "============================================================================" << std::endl;
	test<GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>,
		SquaredMultipleCorrelation<float64_t>>(num_examples, target_feats);

	std::cout << "Test 13: GLS(FR, Linear LS, logdet regularizer) Sum-squared error" << std::endl;
	std::cout << "=================================================================" << std::endl;
	test<GreedyLocalSearch<ForwardRegression,LinearLocalSearch,SmoothedDifferentialEntropy,float64_t>,
		SumSquaredError<float64_t>>(num_examples, target_feats);
	std::cout << "Test 14: GLS(FR, Linear LS, logdet regularizer) Pearson's correlation" << std::endl;
	std::cout << "=====================================================================" << std::endl;
	test<GreedyLocalSearch<ForwardRegression,LinearLocalSearch,SmoothedDifferentialEntropy,float64_t>,
		PearsonsCorrelation<float64_t>>(num_examples, target_feats);
	std::cout << "Test 15: GLS(FR, Linear LS, logdet regularizer) Squared multiple correlation" << std::endl;
	std::cout << "============================================================================" << std::endl;
	test<GreedyLocalSearch<ForwardRegression,LinearLocalSearch,SmoothedDifferentialEntropy,float64_t>,
		SquaredMultipleCorrelation<float64_t>>(num_examples, target_feats);

	return 0;
}
