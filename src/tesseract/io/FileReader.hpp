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

#ifndef FILE_READER_H__
#define FILE_READER_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/**
 * @brief class FileReader for reading and storing features and labels
 */
template <class FeatureReader, class LabelReader>
class FileReader
{
public:
	/** the feature type */
	typedef typename FeatureReader::feat_type feat_type;

	/** the feature vector type */
	typedef typename FeatureReader::vec_type vec_type;

	/** the label type */
	typedef typename LabelReader::label_type label_type;

	/** the data type to be returned by the load method */
	typedef typename std::pair<feat_type, label_type> data_type;

	/** default constructor */
	FileReader();

	/** constructor
	 * @param feats_file the feats filename
	 * @param labels_file the labels filename
	 */
	explicit FileReader(std::string feats_file, std::string labels_file);

	/** destructor */
	~FileReader();

	/** loads the images and labels from the specified files
	 * @param num_examples specifies the number of the images and labels
	 * that are used on the learning problem
	 * @return a pair of feats and labels
	 */
	data_type load(int32_t num_examples);

private:
	/** the feats filename */
	std::string feats_filename;

	/** the labels filename */
	std::string labels_filename;
};

}

#endif // FILE_READER_H__
