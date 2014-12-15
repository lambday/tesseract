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

#include <tesseract/io/HousingReader.hpp>
#include <tesseract/base/init.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace tesseract;

void test1(std::string filename)
{
	HousingReader reader(filename);
	reader.load();

	std::srand(std::time(0));
//	index_t i = std::rand() % reader.get_num_images();
//	std::cout << i << std::endl;

	HousingReader::vec_type feat = reader.get_image(reader.get_num_images()-1);

//	for (float64_t f : feat)
//		printf("%f ", f);
//	std::cout << std::endl;

//	std::cout << reader.get_num_images() << std::endl;
//	std::cout << reader.get_num_labels() << std::endl;
}
/*
void test2(std::string filename)
{
	IDX3Reader reader(filename);
	reader.load();

	IDX3Reader::Image& image = reader.get_image(0);
	index_t count = 0;
	for (index_t i = 1; i < reader.get_num_images(); ++i)
	{
		bool same = true;
		IDX3Reader::Image& current_img = reader.get_image(i);
		assert(image.size() == current_img.size());
		for (index_t j = 0; j < image.size(); ++j)
			same &= image[j] == current_img[j];
		if (!same) count++;
	}
	std::cout << count << std::endl;
}
*/
int main(int argc, char** argv)
{
//	logger.set_loglevel(Debug);
	test1("data/housing.data");
//	test2("data/train-images-idx3-ubyte");
//	test2("data/t10k-images-idx3-ubyte");
	return 0;
}
