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

#ifndef HOUSING_READER_H__
#define HOUSING_READER_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/**
 * @brief class HousingReader for reading and storing Bostom housing data
 */
class HousingReader
{
public:
	/** type of Housing features */
	typedef std::vector<float64_t> HousingFeatures;

	/** type of feature vectors */
	typedef HousingFeatures vec_type;

	/** type of Housing labels */
	typedef std::vector<float64_t> HousingLabels;

	/** the label type */
	typedef std::vector<HousingFeatures> feat_type;

	/** the label type */
	typedef HousingLabels label_type;

	/** default constructor */
	HousingReader();

	/** constructor
	 * @param _filename the filename
	 */
	explicit HousingReader(std::string _filename);

	/** destructor */
	~HousingReader();

	/** loads the images from the specified file */
	void load();

	/** @return a reference of the feature vector at specified index */
	HousingFeatures& get_image(index_t i);

	/** @return the number of feature vectors */
	int32_t get_num_images();

	/** @return a reference of the image at specified index */
	float64_t get_label(index_t i);

	/** @return the number of labels */
	int32_t get_num_labels();

private:
	/** the filename */
	std::string filename;

	/** the features array */
	feat_type feats;

	/** the labels array */
	label_type labels;
};

}

#endif // HOUSING_READER_H__
