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

#ifndef LOCAL_SEARCH_H__
#define LOCAL_SEARCH_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** @brief class LocalSearch for a dummy algorithm which does nothing and returns
 * all the features that are there in the input problem
 */
template <template <class> class Regularizer>
class LocalSearch
{
public:
	/** constructor
	 * @param _regressors the regressors (real valued dense feature matrix)
	 * @param _regressand the regressand (real valued dense labels vector)
	 * @param _eps the epsilon of the algorithm (default value 0.1)
	 */
	LocalSearch(const Matrix<float64_t>& _regressors, const Vector<float64_t>& _regressand,
			float64_t _eps = 22);

	/** destructor */
	~LocalSearch();

	/** @return the vector of selected feature indices */
	std::vector<index_t> run();
private:
	/** real valued dense feature matrix */
	const Matrix<float64_t>& regressors;

	/** real valued dense labels vector */
	const Vector<float64_t>& regressand;

	/** epsilon of the algorithm */
	float64_t eps;
};

}

#endif // LOCAL_SEARCH_H__