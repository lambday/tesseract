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

#include <tesseract/regression/LeastSquares.hpp>
#include <tesseract/base/types.h>

using namespace Eigen;

namespace tesseract
{
template class LeastSquares<MatrixXd, VectorXd, LS_SVD>;
template class LeastSquares<MatrixXf, VectorXf, LS_SVD>;
template class LeastSquares<MatrixXi, VectorXi, LS_SVD>;

template class LeastSquares<MatrixXd, VectorXd, LS_QR>;
template class LeastSquares<MatrixXf, VectorXf, LS_QR>;
template class LeastSquares<MatrixXi, VectorXi, LS_QR>;

template class LeastSquares<MatrixXd, VectorXd, LS_NORMAL>;
template class LeastSquares<MatrixXf, VectorXf, LS_NORMAL>;
template class LeastSquares<MatrixXi, VectorXi, LS_NORMAL>;

template class LeastSquares<Map<MatrixXd>, Map<VectorXd>, LS_SVD>;
template class LeastSquares<Map<MatrixXf>, Map<VectorXf>, LS_SVD>;
template class LeastSquares<Map<MatrixXi>, Map<VectorXi>, LS_SVD>;

template class LeastSquares<Map<MatrixXd>, Map<VectorXd>, LS_QR>;
template class LeastSquares<Map<MatrixXf>, Map<VectorXf>, LS_QR>;
template class LeastSquares<Map<MatrixXi>, Map<VectorXi>, LS_QR>;

template class LeastSquares<Map<MatrixXd>, Map<VectorXd>, LS_NORMAL>;
template class LeastSquares<Map<MatrixXf>, Map<VectorXf>, LS_NORMAL>;
template class LeastSquares<Map<MatrixXi>, Map<VectorXi>, LS_NORMAL>;
}
