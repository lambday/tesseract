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

#ifndef IDX1_READER_H__
#define IDX1_READER_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/**
 * @brief struct IDX1Header for the header of the image file
 */
struct IDX1Header
{
	/** magic number (should be 2051 or 0x803) */
	int32_t magic_number;

	/** number of labels in current file */
	int32_t num_labels;
};

/**
 * @brief class IDX1Reader for reading and storing IDX1 images
 */
class IDX1Reader
{
public:
	/** type of IDX1 images array */
	typedef std::vector<ubyte_t> IDX1Labels;

	/** default constructor */
	IDX1Reader();

	/** constructor
	 * @param _filename the filename
	 */
	explicit IDX1Reader(std::string _filename);

	/** destructor */
	~IDX1Reader();

	/** loads the images from the specified file */
	void load();

	/** @return a reference of the image at specified index */
	ubyte_t get_label(index_t i);

	/** @return the number of labels */
	int32_t get_num_labels();

private:
	/** the filename */
	std::string filename;

	/** the image header */
	IDX1Header header;

	/** the images array */
	IDX1Labels labels;
};

}

#endif // IDX1_READER_H__
