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
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <endian.h>

using namespace tesseract;

IDX1Reader::IDX1Reader()
{
}

IDX1Reader::IDX1Reader(std::string _filename) : filename(_filename)
{
}

IDX1Reader::~IDX1Reader()
{
}

void IDX1Reader::load()
{
	assert(!filename.empty());

	std::basic_ifstream<byte_t> is;
	is.open(filename, std::ios::in | std::ios::binary);

	if (is.is_open()) {

		// read header info
		is.read(reinterpret_cast<byte_t*>(&header.magic_number), 4);
		header.magic_number = be32toh(header.magic_number);

		is.read(reinterpret_cast<byte_t*>(&header.num_labels), 4);
		header.num_labels = be32toh(header.num_labels);

		// allocate memory
		int32_t bufsize = header.num_labels;
		byte_t *buffer = new byte_t[bufsize];

		labels.resize(header.num_labels);

		// read the labels
		is.read(buffer, bufsize);
		std::copy(buffer, buffer + bufsize, labels.begin());

		is.close();

		delete buffer;
	} else {
		std::cerr << "Could not open file " << filename << std::endl;
		exit(2);
	}
}

ubyte_t IDX1Reader::get_label(index_t i)
{
	return labels[i];
}

int32_t IDX1Reader::get_num_labels()
{
	return header.num_labels;
}
