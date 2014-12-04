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

#include <tesseract/preprocessor/DataGenerator.hpp>
#include <tesseract/io/FileReader.hpp>
#include <tesseract/io/IDX1Reader.hpp>
#include <tesseract/io/IDX3Reader.hpp>
#include <cstdlib>

using namespace tesseract;

template <class FeatureReader, class LabelReader, class Normalizer>
DataGenerator<FeatureReader,LabelReader,Normalizer>::DataGenerator()
{
}

template <class FeatureReader, class LabelReader, class Normalizer>
DataGenerator<FeatureReader,LabelReader,Normalizer>::DataGenerator(std::string feats_file,
		std::string labels_file)
	: feats_filename(feats_file), labels_filename(labels_file)
{
}

template <class FeatureReader, class LabelReader, class Normalizer>
DataGenerator<FeatureReader,LabelReader,Normalizer>::~DataGenerator()
{
}

template <class FeatureReader, class LabelReader, class Normalizer>
void DataGenerator<FeatureReader,LabelReader,Normalizer>::generate()
{
	typedef FileReader<FeatureReader,LabelReader> Reader;
	typedef typename Reader::feat_type feat_type;
	typedef typename Reader::label_type label_type;
	typedef typename Reader::data_type data_type;

	// set seed for random sampling
	std::srand(seed);

	// sample num_examples examples
	Reader reader(feats_filename, labels_filename);
	const data_type& examples = reader.load(num_examples);

	const feat_type& features = examples.first;
	const label_type& labels = examples.second;

	// sanity check
	assert(features.size() > 0);
	assert(features[0].size() > 0);
	assert(labels.size() > 0);

	// get dimension of the regressors, assumes that all feature vectors
	// are of equal length
	int32_t num_vec = features.size();
	int32_t num_feats = features[0].size();

	// further sanity check for number of labels
	assert(num_vec == labels.size());

	// allocate and copy feature matrix
	regressors = Matrix<float64_t>(num_vec, num_feats);
	for (index_t i = 0; i < regressors.rows(); ++i)
	{
		const std::vector<ubyte_t>& current = features[i];
		for (index_t j = 0; j < regressors.cols(); ++j)
		{
			regressors(i, j) = static_cast<float64_t>(current[j]);
		}
	}

	// allocate and copy labels vector
	regressand = Vector<float64_t>(num_vec);
	for (index_t i = 0; i < regressand.rows(); ++i)
	{
		regressand[i] = static_cast<float64_t>(labels[i]);
	}

	// normalize the regressors and regressands
	// TODO
}

template <class FeatureReader, class LabelReader, class Normalizer>
void DataGenerator<FeatureReader,LabelReader,Normalizer>::set_num_examples(int32_t _num_examples)
{
	num_examples = _num_examples;
}

template <class FeatureReader, class LabelReader, class Normalizer>
int32_t DataGenerator<FeatureReader,LabelReader,Normalizer>::get_num_examples() const
{
	return num_examples;
}

template <class FeatureReader, class LabelReader, class Normalizer>
void DataGenerator<FeatureReader,LabelReader,Normalizer>::set_seed(int32_t _seed)
{
	seed = _seed;
}

template <class FeatureReader, class LabelReader, class Normalizer>
const Matrix<float64_t>& DataGenerator<FeatureReader,LabelReader,Normalizer>::get_regressors() const
{
	return regressors;
}

template <class FeatureReader, class LabelReader, class Normalizer>
const Vector<float64_t>& DataGenerator<FeatureReader,LabelReader,Normalizer>::get_regressand() const
{
	return regressand;
}

template class DataGenerator<IDX3Reader, IDX1Reader, int>; // TODO used int for dummy
