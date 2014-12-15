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

#include <tesseract/io/FileReader.hpp>
#include <tesseract/io/IDX3Reader.hpp>
#include <tesseract/io/IDX1Reader.hpp>
#include <tesseract/io/HousingReader.hpp>
#include <algorithm>
#include <map>
#include <vector>
#include <cstdlib>

using namespace tesseract;

template <class FeatureReader, class LabelReader>
FileReader<FeatureReader, LabelReader>::FileReader()
{
}

template <class FeatureReader, class LabelReader>
FileReader<FeatureReader, LabelReader>::FileReader(std::string feats_file, std::string labels_file)
	: feats_filename(feats_file), labels_filename(labels_file)
{
}

template <class FeatureReader, class LabelReader>
FileReader<FeatureReader, LabelReader>::~FileReader()
{
}

template <class FeatureReader, class LabelReader>
typename FileReader<FeatureReader,LabelReader>::data_type
FileReader<FeatureReader,LabelReader>::load(int32_t num_examples)
{
	// create new feature matrix
	typename FeatureReader::feat_type features;

	// make sure to use the same indices for labels as well
	std::vector<int32_t> indices(num_examples);

	// read all the features
	{
		FeatureReader feats_reader(feats_filename);
		feats_reader.load();

		assert(feats_reader.get_num_images() >= num_examples);

		// generate random problem instance
		std::for_each(indices.begin(), indices.end(), [&feats_reader](int32_t& val)
		{
			val = std::rand() % feats_reader.get_num_images();
		});

		// sort the indices
		std::sort(indices.begin(), indices.end());

		features.resize(indices.size());
		for (index_t i = 0; i < indices.size(); ++i)
		{
			features[i] = feats_reader.get_image(indices[i]);
		}
	}

	// create new labels vector
	typename LabelReader::label_type labels;

	// read all the labels
	{
		LabelReader label_reader(labels_filename);
		label_reader.load();

		assert(label_reader.get_num_labels() >= num_examples);

		labels.resize(indices.size());
		for (index_t i = 0; i < indices.size(); ++i)
		{
			labels[i] = label_reader.get_label(indices[i]);
		}
	}

	return std::make_pair(features, labels);
}

template class FileReader<IDX3Reader, IDX1Reader>;
template class FileReader<HousingReader, HousingReader>;
