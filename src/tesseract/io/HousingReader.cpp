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
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace tesseract;

HousingReader::HousingReader()
{
}

HousingReader::HousingReader(std::string _filename) : filename(_filename)
{
}

HousingReader::~HousingReader()
{
}

void HousingReader::load()
{
	assert(!filename.empty());

	std::basic_ifstream<byte_t> is;
	is.open(filename, std::ios::in);

	index_t num_rows = 506;
	index_t num_cols = 14;
	Matrix<float64_t> data(num_rows, num_cols);

	if (is.is_open()) {

		index_t i = 0;
		while (!is.eof() && i < num_rows)
		{
			logger.write(Debug, "Reading row %d\n", i);

			std::string str;
			std::getline(is, str);

			logger.write(Debug, "%s\n", str.c_str());

			std::stringstream ss(str);

			for (index_t j = 0; j < num_cols && ss >> data(i,j); ++j)
			{
				logger.write(Debug, "%f ", data(i,j));
			}
			i++;
		}

		if (logger.get_loglevel() >= Debug)
		{
			logger.print_matrix(data);
		}

		// allocate memory
		feats.resize(num_rows);
		for (index_t k = 0; k < num_rows; ++k)
		{
			feats[k].resize(num_cols - 1);
		}
		labels.resize(num_rows);

		// copy the features and the labels
		for (index_t k = 0; k < num_rows; ++k)
		{
			for (index_t j = 0; j < num_cols - 1; ++j)
			{
				feats[k][j] = data(k, j);
			}
			labels[k] = data(k, num_cols-1);
		}

		is.close();
	} else {
		std::cerr << "Could not open file " << filename << std::endl;
		exit(2);
	}
}

HousingReader::HousingFeatures& HousingReader::get_image(index_t i)
{
	return feats[i];
}

float64_t HousingReader::get_label(index_t i)
{
	return labels[i];
}

int32_t HousingReader::get_num_images()
{
	return feats.size();
}

int32_t HousingReader::get_num_labels()
{
	return labels.size();
}
