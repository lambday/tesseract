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

#include <tesseract/io/IDX3Reader.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <endian.h>

using namespace tesseract;

IDX3Reader::IDX3Reader()
{
}

IDX3Reader::IDX3Reader(std::string _filename) : filename(_filename)
{
}

IDX3Reader::~IDX3Reader()
{
}

void IDX3Reader::load()
{
	assert(!filename.empty());

	std::basic_ifstream<byte_t> is;
	is.open(filename, std::ios::in | std::ios::binary);

	if (is.is_open()) {

		// read header info
		is.read(reinterpret_cast<byte_t*>(&header.magic_number), 4);
		header.magic_number = be32toh(header.magic_number);

		is.read(reinterpret_cast<byte_t*>(&header.num_images), 4);
		header.num_images = be32toh(header.num_images);

		is.read(reinterpret_cast<byte_t*>(&header.num_rows), 4);
		header.num_rows = be32toh(header.num_rows);
		is.read(reinterpret_cast<byte_t*>(&header.num_cols), 4);
		header.num_cols = be32toh(header.num_cols);

		// allocate memory
		int32_t dim = header.num_rows * header.num_cols;
		int32_t bufsize = header.num_images * dim;
		byte_t *buffer = new byte_t[bufsize];

		images.resize(header.num_images);
		std::for_each(images.begin(), images.end(), [&dim](decltype(images[0]) img)
		{
			img.resize(dim);
		});

		// read the images
		is.read(buffer, bufsize);
		for (index_t i = 0; i < header.num_images; ++i)
		{
			std::copy(buffer + i*dim, buffer + (i+1)*dim, images[i].begin());
		}

		is.close();

		delete buffer;
	} else {
		std::cerr << "Could not open file " << filename << std::endl;
		exit(2);
	}
}

IDX3Reader::Image& IDX3Reader::get_image(index_t i)
{
	return images[i];
}

int32_t IDX3Reader::get_num_images()
{
	return header.num_images;
}

int32_t IDX3Reader::get_num_rows()
{
	return header.num_rows;
}

int32_t IDX3Reader::get_num_cols()
{
	return header.num_cols;
}
