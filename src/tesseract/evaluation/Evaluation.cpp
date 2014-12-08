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

#include <tesseract/evaluation/Evaluation.hpp>
#include <tesseract/evaluation/DataSet.hpp>
#include <tesseract/preprocessor/DataGenerator.hpp>
#include <tesseract/io/IDX1Reader.hpp>
#include <tesseract/io/IDX3Reader.hpp>
#include <tesseract/normalizer/UnitL2Normalizer.hpp>
#include <tesseract/errors/SumSquaredError.hpp>
#include <tesseract/errors/PearsonsCorrelation.hpp>
#include <tesseract/errors/SquaredMultipleCorrelation.hpp>
#include <tesseract/algorithm/Dummy.hpp>
#include <tesseract/algorithm/ForwardRegression.hpp>
#include <tesseract/algorithm/LocalSearch.hpp>
#include <tesseract/algorithm/GreedyLocalSearch.hpp>
#include <tesseract/regression/LeastSquares.hpp>
#include <tesseract/regularizer/DummyRegularizer.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <tesseract/features/Features.hpp>
#include <map>
#include <type_traits>

using namespace tesseract;

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
std::vector<index_t> Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::train()
{
	// the covariance matrix required by the algorithm
	Matrix<float64_t> cov;

	// data generator is in the block because its destruction frees the
	// memory holding the dataset
	{
		// read training data
		DataGenerator gen(DataSet::feat_train, DataSet::label_train);
		gen.set_seed(seed);
		gen.set_num_examples(num_examples);
		gen.generate();
		cov = gen.get_cov();
	}

	// only cov matrix is in memory

	// run algorithm
	Algorithm algo(cov, target_feats);
	algo.set_params(params);

	return algo.run().second;
}

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
std::pair<index_t,float64_t> Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::test(std::vector<index_t> indices)
{
	// read test data
	DataGenerator gen(DataSet::feat_test, DataSet::label_test);
	gen.set_seed(seed);
	gen.set_num_examples(num_examples);
	gen.generate();

	// copy only selected features
	const Eigen::Ref<const Matrix<float64_t>> feats_in_use =
		Features<float64_t>::copy_feats(gen.get_regressors(), indices);

	Vector<float64_t> coeff = Vector<float64_t>::Zero(indices.size());

	// fit least square on test data for the selected features
	LeastSquares<float64_t, LS_NORMAL> model;
	model.solve(feats_in_use, gen.get_regressand(), coeff);

	// compute error measure
	ErrorMeasure measure;
	return std::make_pair(indices.size(),measure.compute(gen.get_regressand(), feats_in_use*coeff));
}

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
std::pair<index_t,float64_t> Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::evaluate()
{
	return test(train());
}

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
void Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::set_params(typename
		Algorithm::param_type _params)
{
	params = _params;
}

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
void Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::set_seed(int32_t _seed)
{
	seed = _seed;
}

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
void Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::set_num_examples(index_t _num_examples)
{
	num_examples = _num_examples;
}

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
void Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::set_target_feats(index_t _target_feats)
{
	target_feats = _target_feats;
}

template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 Dummy,SumSquaredError<float64_t>>;
template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 Dummy,PearsonsCorrelation<float64_t>>;
template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 Dummy,SquaredMultipleCorrelation<float64_t>>;

template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 ForwardRegression<DummyRegularizer,float64_t>,SumSquaredError<float64_t>>;
template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 ForwardRegression<DummyRegularizer,float64_t>,PearsonsCorrelation<float64_t>>;
template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 ForwardRegression<DummyRegularizer,float64_t>,SquaredMultipleCorrelation<float64_t>>;

template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 ForwardRegression<SmoothedDifferentialEntropy,float64_t>,SumSquaredError<float64_t>>;
template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 ForwardRegression<SmoothedDifferentialEntropy,float64_t>,PearsonsCorrelation<float64_t>>;
template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 ForwardRegression<SmoothedDifferentialEntropy,float64_t>,SquaredMultipleCorrelation<float64_t>>;

template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>,
		 SumSquaredError<float64_t>>;
template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>,
		 PearsonsCorrelation<float64_t>>;
template class Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		 GreedyLocalSearch<ForwardRegression,LocalSearch,SmoothedDifferentialEntropy,float64_t>,
		 SquaredMultipleCorrelation<float64_t>>;
