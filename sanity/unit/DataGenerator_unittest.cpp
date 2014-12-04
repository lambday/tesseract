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
#include <tesseract/io/IDX3Reader.hpp>
#include <tesseract/io/FileReader.hpp>
#include <tesseract/preprocessor/DataGenerator.hpp>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <iostream>
#include <cstdio>

using namespace tesseract;

void test1(std::string feats_filename, std::string labels_filename)
{
	DataGenerator<IDX3Reader, IDX1Reader, int> gen(feats_filename, labels_filename);
	int32_t num_examples = 2;
	gen.set_num_examples(num_examples);
	gen.set_seed(std::time(0));

	gen.generate();
	const Matrix<float64_t>& regressors = gen.get_regressors();
	const Vector<float64_t>& regressand = gen.get_regressand();

	std::cout << regressors << std::endl;
	std::cout << regressand << std::endl;

}

int main(int argc, char** argv)
{
	test1("data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte");
	test1("data/t10k-images-idx3-ubyte", "data/t10k-labels-idx1-ubyte");
	return 0;
}
