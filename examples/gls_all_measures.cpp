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
#include <ctime>
#include <map>
#include <sys/time.h>
#include <iostream>

using namespace tesseract;
using namespace Eigen;

float64_t get_cpu_time()
{
	return clock();
}

/*
float64_t get_wall_time()
{
	return gettimeofday();
}
*/

// print statistics
	// 1. number of target features
	// 2. eta
	// 3. epsilon
	// 4. delta of the regularizer
	// 5. number of selected features
//--	// 6. wall clock time for training algorithm
	// 7. cpu time for training algorithm
	// 8. optimal function value found by the algorithm
	// 9. sum squared error
	// 10. Pearson's correlation
	// 11. R^2 statistic

typedef GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>::param_type param_type;
typedef SmoothedDifferentialEntropy<float64_t>::param_type reg_param_type;
typedef LocalSearch<SmoothedDifferentialEntropy,float64_t>::param_type ls_param_type;

MatrixXd get_training_data_cov(index_t num_examples)
{
	DataGenerator<IDX3Reader, IDX1Reader, UnitL2Normalizer> gen(MNISTDataSet::feat_train, MNISTDataSet::label_train);
	gen.set_seed(12345);
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
	// 9. sum squared error
	// 10. Pearson's correlation
	// 11. R^2 statistic
	printf("%.6f %.6f %.6f\n", measure_1.compute(regressand, Zp),
			measure_2.compute(regressand, Zp), measure_3.compute(regressand, Zp));
}

void train_test(const Ref<const MatrixXd>& cov, param_type params, index_t target_feats,
		const Ref<const MatrixXd>& regressors, const Ref<const VectorXd>& regressand)
{
	// initialize the algo and set up parameters
	GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t> algo(cov, target_feats);
	algo.set_params(params);

	// set timers
//	float64_t wall0 = get_wall_time();
	float64_t cpu0 = get_cpu_time();

	// run algo
	std::pair<float64_t,std::vector<index_t>> ret = algo.run();

	// stop timers
//	float64_t wall1 = get_wall_time();
	float64_t cpu1 = get_cpu_time();

//	float64_t wall_elapsed = wall1 - wall0;
	float64_t cpu_elapsed = cpu1 - cpu0;

	// print statistics
	// format
	// 5. number of selected features
//--	// 6. wall clock time for training algorithm
	// 7. cpu time for training algorithm
	// 8. optimal function value found by the algorithm
	printf("%u %.6fs %.6f ", ret.second.size(), cpu_elapsed/1E6, ret.first);

	test(ret.second, regressors, regressand);
}

int main(int argc, char** argv)
{
	// algorithm parameters
	float64_t eta = 0.00001;
	float64_t eps = 2;
	float64_t delta = 0.1;

	index_t num_examples = 1000;

	index_t min_feats = 10;
	index_t max_feats = 50;

	// store cov once and run the algo for different number of feats
	MatrixXd cov = get_training_data_cov(num_examples);
	param_type params = get_params(eta, eps, delta);

	// generate testdata
	DataGenerator<IDX3Reader, IDX1Reader, UnitL2Normalizer> gen(MNISTDataSet::feat_test, MNISTDataSet::label_test);
	gen.set_seed(12345);
	gen.set_num_examples(num_examples);
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
