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
#include <tesseract/algorithm/Dummy.hpp>
#include <tesseract/regression/LeastSquares.hpp>
#include <tesseract/features/Features.hpp>
#include <map>
#include <type_traits>

using namespace tesseract;

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
std::vector<index_t> Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::train()
{
	// read training data
	DataGenerator gen(DataSet::feat_train, DataSet::label_train);
	gen.set_seed(seed);
	gen.set_num_examples(num_examples);
	gen.generate();

	// run algorithm
	Algorithm algo(gen.get_regressors(), gen.get_regressand(), target_feats);
	return algo.run();
}

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
std::pair<index_t,float64_t> Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::test(std::vector<index_t> indices)
{
	// read test data
	DataGenerator gen(DataSet::feat_test, DataSet::label_test);
	gen.set_seed(seed);
	gen.set_num_examples(num_examples);
	gen.generate();

	typedef typename std::remove_cv<typename std::remove_reference<decltype(gen.get_regressors())>::type>::type feat_type;
	typedef typename std::remove_cv<typename std::remove_reference<decltype(gen.get_regressand())>::type>::type label_type;

	// fit least square on test data for the selected features
	LeastSquares<feat_type, label_type, LS_SVD> model;
	const feat_type& feats_in_use = Features<float64_t>::copy_dimension_subset(gen.get_regressors(), indices);
	label_type result(num_examples);
	model.solve(feats_in_use, gen.get_regressand(), result);

	// compute error measure
	ErrorMeasure measure;
	return std::make_pair(indices.size(),measure.compute(gen.get_regressand(), result));
}

template <class DataSet, class DataGenerator, class Algorithm, class ErrorMeasure>
std::pair<index_t,float64_t> Evaluation<DataSet,DataGenerator,Algorithm,ErrorMeasure>::evaluate()
{
	return test(train());
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
