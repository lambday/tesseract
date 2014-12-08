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

#ifndef FEATURES_H__
#define FEATURES_H__

#include <tesseract/base/types.h>

namespace tesseract
{
/** @brief template class Features provides static methods for feature manipulation */
template <typename T>
struct Features
{
	/** copies the specified dimensions into a new matrix object
	 * @param m the feature matrix
	 * @param inds the indices which are to be copied
	 * @return the new feature matrix with columns as specified by the indices param
	 */
	static Matrix<T> copy_feats(const Eigen::Ref<const Matrix<T>>& m,
			std::vector<index_t>& inds);

	/** copies the specified dimensions into a new matrix object and appends that
	 * with a column vector
	 * @param m the feature matrix
	 * @param v the column vector to be appended at the end (this would be the last
	 * column of the result matrix
	 * @param inds the indices which are to be copied
	 * @return the new feature matrix with columns as specified by the indices param
	 */
	static Matrix<T> copy_feats(const Eigen::Ref<const Matrix<T>>& m,
			const Eigen::Ref<const Vector<T>>& v, std::vector<index_t>& inds);

	/** copies the specified subset of the covariance matrix
	 * @param cov the whole covariance matrix C
	 * @param inds the indices defining the subset S
	 * @return the submatrix of C containing only S, C_S
	 */
	static Matrix<T> copy_cov(const Eigen::Ref<const Matrix<T>>& cov,
			std::vector<index_t>& inds);
};

}

#endif // FEATURES_H__
