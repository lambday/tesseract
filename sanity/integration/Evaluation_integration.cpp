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
#include <tesseract/preprocessor/DataGenerator.hpp>
#include <tesseract/normalizer/UnitL2Normalizer.hpp>
#include <tesseract/algorithm/Dummy.hpp>
#include <tesseract/errors/SumSquaredError.hpp>
#include <tesseract/errors/PearsonsCorrelation.hpp>
#include <tesseract/evaluation/Evaluation.hpp>
#include <tesseract/evaluation/DataSet.hpp>
#include <ctime>
#include <map>
#include <iostream>

using namespace tesseract;

template <class Algorithm, class ErrorMeasures>
void test()
{
	Evaluation<MNISTDataSet,DataGenerator<IDX3Reader,IDX1Reader,UnitL2Normalizer>,
		Algorithm,ErrorMeasures> evaluator;
	evaluator.set_seed(std::time(0));
	evaluator.set_num_examples(1000);
	evaluator.set_target_feats(0);

	std::pair<index_t,float64_t> result = evaluator.evaluate();

	std::cout << result.first << std::endl;
	std::cout << result.second << std::endl;
}

int main(int argc, char** argv)
{
	test<Dummy,SumSquaredError<float64_t>>();
	test<Dummy,PearsonsCorrelation<float64_t>>();
	return 0;
}
