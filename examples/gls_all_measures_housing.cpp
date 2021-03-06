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
#include <tesseract/io/HousingReader.hpp>
#include <tesseract/preprocessor/DataGenerator.hpp>
#include <tesseract/normalizer/UnitL2Normalizer.hpp>
#include <tesseract/algorithm/ForwardRegression.hpp>
#include <tesseract/algorithm/LocalSearch.hpp>
#include <tesseract/algorithm/GreedyLocalSearch.hpp>
#include <tesseract/errors/SumSquaredError.hpp>
#include <tesseract/errors/PearsonsCorrelation.hpp>
#include <tesseract/errors/SquaredMultipleCorrelation.hpp>
#include <tesseract/evaluation/DataSet.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <tesseract/regression/LeastSquares.hpp>
#include <tesseract/features/Features.hpp>
#include <map>
#include <iostream>
#include <chrono>

using namespace tesseract;
using namespace Eigen;

// print statistics
	// 1. number of target features
	// 2. eta
	// 3. epsilon
	// 4. delta of the regularizer
	// 5. number of selected features
	// 6. cpu time for training algorithm
	// 7. optimal function value found by the algorithm
	// 8. sum squared error
	// 9. Pearson's correlation
	// 10. R^2 statistic

typedef GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>::param_type param_type;
typedef SmoothedDifferentialEntropy<float64_t>::param_type reg_param_type;
typedef LocalSearch<SmoothedDifferentialEntropy,float64_t>::param_type ls_param_type;

MatrixXd get_training_data_cov(index_t num_examples)
{
	DataGenerator<HousingReader, HousingReader, UnitL2Normalizer> gen(HousingDataSet::feat_train, HousingDataSet::label_train);
	gen.set_seed(100);
	gen.set_num_examples(num_examples);
	gen.generate();
	return gen.get_cov();
}

param_type get_params(float64_t eta, float64_t eps, float64_t delta)
{
	// specify the parameters via underlying local search parameters

	// regularizer parameter delta for smoothed differential entropy
	reg_param_type reg_params(delta);

	// local search parameter contains eta for objective function computation (regularization const)
	// eps for local search itself and underlying regularizer parameters
	param_type params(ls_param_type(eta,eps,reg_params));

	return params;
}

void test(std::vector<index_t> inds, const Ref<const MatrixXd>& regressors, const Ref<const VectorXd>& regressand)
{
	// copy only selected features
	MatrixXd feats_in_use = Features<float64_t>::copy_feats(regressors, inds);
	VectorXd coeff = VectorXd::Zero(inds.size());

	// fit least square on test data for the selected features
	LeastSquares<float64_t, LS_NORMAL> model;
	model.solve(feats_in_use, regressand, coeff);

	VectorXd Zp = feats_in_use * coeff;

	// compute error measure
	SumSquaredError<float64_t> measure_1;
	PearsonsCorrelation<float64_t> measure_2;
	SquaredMultipleCorrelation<float64_t> measure_3;

	// print statistics
	// format
	// 8. sum squared error
	// 9. Pearson's correlation
	// 10. R^2 statistic
	printf("%.10f %.10f %.10f\n", measure_1.compute(regressand, Zp),
			measure_2.compute(regressand, Zp), measure_3.compute(regressand, Zp));
}

void train_test(const Ref<const MatrixXd>& cov, param_type params, index_t target_feats,
		const Ref<const MatrixXd>& regressors, const Ref<const VectorXd>& regressand)
{
	// initialize the algo and set up parameters
	GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t> algo(cov, target_feats);
	algo.set_params(params);

	// set timers
	const auto cpu0 = std::chrono::high_resolution_clock::now();

	// run algo
	std::pair<float64_t,std::vector<index_t>> ret = algo.run();

	// stop timers
	const auto cpu1 = std::chrono::high_resolution_clock::now();
	const auto cpu_elapsed = cpu1 - cpu0;

	// print statistics
	// format
	// 5. number of selected features
	// 6. cpu time for training algorithm
	// 7. optimal function value found by the algorithm
	printf("%u %.10f %.10f ", ret.second.size(), std::chrono::duration<float64_t>(cpu_elapsed).count(), ret.first);

	test(ret.second, regressors, regressand);
}

int main(int argc, char** argv)
{
	logger.set_loglevel(Special);

	// algorithm parameters
	float64_t eta = 0.01;
	float64_t eps = 1.0;
	float64_t delta = 0.00001;

	index_t num_examples = 500;
	index_t num_test_examples = 100;

	index_t min_feats = 4;
	index_t max_feats = 10;

	// store cov once and run the algo for different number of feats
	MatrixXd cov = get_training_data_cov(num_examples);
	param_type params = get_params(eta, eps, delta);

	// generate testdata
	DataGenerator<HousingReader, HousingReader, UnitL2Normalizer> gen(HousingDataSet::feat_test, HousingDataSet::label_test);
	gen.set_seed(12345);
	gen.set_num_examples(num_test_examples);
	gen.generate();

	for (index_t i = min_feats; i <= max_feats; ++i)
	{
		// 1. number of target features
		// 2. eta
		// 3. epsilon
		// 4. delta of the regularizer
		printf("%u %f %f %f ", i, eta, eps, delta);
		train_test(cov, params, i, gen.get_regressors(), gen.get_regressand());
	}

	return 0;
}
