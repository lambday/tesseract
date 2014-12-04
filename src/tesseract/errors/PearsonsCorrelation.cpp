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

#include <tesseract/errors/PearsonsCorrelation.hpp>
#include <cmath>

using namespace tesseract;

template <typename T>
float64_t PearsonsCorrelation<T>::compute(const Vector<T>& Z, const Vector<T>& Zp)
{
	assert(Z.rows() == Zp.rows());
	assert(Z.rows() > 2);

	int32_t N = Z.rows();

	// compute mean
	Vector<float64_t> mu_Z = Vector<float64_t>::Constant(N, Z.array().template sum() * 1.0 / N);
	Vector<float64_t> mu_Zp = Vector<float64_t>::Constant(N, Zp.array().template sum() * 1.0 / N);

	// compute sample std dev
	float64_t s_Z = sqrt((Z - mu_Z).array().template square().template sum() / (N - 1.0));
	float64_t s_Zp = sqrt((Zp - mu_Zp).array().template square().template sum() / (N - 1.0));

	// compute Pearson product-moment correlation coefficient
	float64_t r = (((Z - mu_Z) / s_Z).array() * ((Zp - mu_Zp) / s_Zp).array()).template sum() / (N - 1);

	// compute Pearson's correlation
	float64_t SSY = (Z - mu_Z).array().template square().template sum();
	return sqrt((1 - r*r) * SSY / (N - 2));
}

template class PearsonsCorrelation<float64_t>;
