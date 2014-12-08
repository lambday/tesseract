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

#include <tesseract/features/Features.hpp>

using namespace tesseract;

template <typename T>
Matrix<T> Features<T>::copy_feats(const Eigen::Ref<const Matrix<T>>& m, std::vector<index_t>& inds)
{
	if (inds.size() == m.cols())
		return m;

	Matrix<T> mat(m.rows(), inds.size());
	std::sort(inds.begin(), inds.end());

	for (index_t i = 0; i < inds.size(); ++i)
	{
		mat.col(i) = m.col(inds[i]);
	}

	return mat;
}

template <typename T>
Matrix<T> Features<T>::copy_feats(const Eigen::Ref<const Matrix<T>>& m,
		const Eigen::Ref<const Vector<T>>& v, std::vector<index_t>& inds)
{
	Matrix<T> mat(m.rows(), inds.size() + 1);
	std::sort(inds.begin(), inds.end());

	assert(m.rows() == v.rows());

	for (index_t i = 0; i < inds.size(); ++i)
	{
		mat.col(i) = m.col(inds[i]);
	}
	mat.col(mat.cols() - 1) = v;

	return mat;
}

template <typename T>
Matrix<T> Features<T>::copy_cov(const Eigen::Ref<const Matrix<T>>& cov, std::vector<index_t>& inds)
{
	// sanity check
	assert(cov.rows() == cov.cols());
	std::sort(inds.begin(), inds.end());
	assert(inds[0] >= 0 && inds[inds.size()-1] < cov.cols());

	Matrix<T> c(inds.size(), inds.size());

	// copying index-wise, slow but memory efficient
	// since we are caching the whole covariance matrix, it reduces the computation
	// time and memory requirement
	for (index_t i = 0; i < c.rows(); ++i)
	{
		// make use of symmetry
		for (index_t j = 0; j < i; ++j)
		{
			c(i, j) = c(j, i) = cov(inds[i], inds[j]);
		}

		// copy the diagonal
		c(i, i) = cov(inds[i], inds[i]);
	}

	return c;
}

template class Features<float64_t>;
