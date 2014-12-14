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

#ifndef DATA_GENERATOR_H__
#define DATA_GENERATOR_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** perturbation type */
enum PerturbationType
{
	/** no perturbation */
	NoPerturbation,
	/** add noise to the regressors */
	Regressors,
	/** add noise to the regressand */
	Regressand,
	/** add noise to both regressors and regressand */
	Both
};

/** @brief template class DataGenerator which generates test data.
 * It reads the features and labels for given number of examples
 * inside it's generate method using FeatureReader and LabelReader
 * and then performs a normalization on the data (both features and
 * labels) using the Normalizer. The generated data is stored inside
 * as real valued Eigen3 matrix and vector format.
 */
template <class FeatureReader, class LabelReader, template <class> class Normalizer>
class DataGenerator
{
public:
	/** default constructor */
	DataGenerator();

	/** constructor */
	explicit DataGenerator(std::string feats_file, std::string labels_file);

	/** destructor */
	~DataGenerator();

	/*
	 * It reads the features and labels for given number of examples
	 * inside it's generate method using FeatureReader and LabelReader
	 * and then performs a normalization on the data (both features and
	 * labels) using the Normalizer. The generated data is stored inside
	 * as real valued Eigen3 matrix and vector format.
	 */
	void generate();

	/** @param _seed for rng */
	void set_seed(int32_t _seed);

	/** @param _perturbation_type the perturbation type */
	void set_perturbation_type(PerturbationType _perturbation_type);

	/** @param _sigma the sigma for perturbation */
	void set_sigma(float64_t _sigma);

	/** @param _num_examples the number of examples */
	void set_num_examples(int32_t _num_examples);

	/** @return the number of examples */
	int32_t get_num_examples() const;

	/** @return the covariance matrix on the whole data */
	const Matrix<float64_t> get_cov() const;

	/** @return the regressors (real valued dense feature matrix) */
	const Eigen::Ref<const Matrix<float64_t>> get_regressors() const;

	/** @return the regressand (real valued dense label vector) */
	const Eigen::Ref<const Vector<float64_t>> get_regressand() const;

private:
	/** perturbate the data */
	void perturbate();

	/** the regressors (all but last column) and regressand (last column) */
	Matrix<float64_t> data;

	/** number of examples */
	int32_t num_examples;

	/** seed for random sampling */
	int32_t seed;

	/** feature filename */
	std::string feats_filename;

	/** labels filename */
	std::string labels_filename;

	/** perturbation type */
	PerturbationType perturbation_type;

	/** sigma for the perturbation */
	float64_t sigma;
};

}

#endif // DATA_GENERATOR_H__
