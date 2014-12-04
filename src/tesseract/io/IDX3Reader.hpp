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

#ifndef IDX3_READER_H__
#define IDX3_READER_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/**
 * @brief struct IDX3Header for the header of the image file
 */
struct IDX3Header
{
	/** magic number (should be 2051 or 0x803) */
	int32_t magic_number;

	/** number of images in current file */
	int32_t num_images;

	/** number of rows of the images */
	int32_t num_rows;

	/** number of cols of the images */
	int32_t num_cols;
};

/**
 * @brief class IDX3Reader for reading and storing IDX3 images
 */
class IDX3Reader
{
public:
	/** type of IDX3 images */
	typedef std::vector<ubyte_t> Image;

	/** type of IDX3 images array */
	typedef std::vector<Image> IDX3Images;

	/** default constructor */
	IDX3Reader();

	/** constructor
	 * @param _filename the filename
	 */
	explicit IDX3Reader(std::string _filename);

	/** destructor */
	~IDX3Reader();

	/** loads the images from the specified file */
	void load();

	/** @return a reference of the image at specified index */
	Image& get_image(index_t i);

	/** @return the number of images */
	int32_t get_num_images();

	/** @return the number of rows for each image */
	int32_t get_num_rows();

	/** @return the number of columns for each image */
	int32_t get_num_cols();

private:
	/** the filename */
	std::string filename;

	/** the image header */
	IDX3Header header;

	/** the images array */
	IDX3Images images;
};

}

#endif // IDX3_READER_H__
