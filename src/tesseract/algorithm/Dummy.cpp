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

#include <tesseract/algorithm/Dummy.hpp>
#include <algorithm>
#include <map>

using namespace tesseract;

Dummy::Dummy(const Eigen::Ref<const Matrix<float64_t>>& _cov, index_t _target_feats)
	: cov(_cov), target_feats(_target_feats)
{
}

Dummy::~Dummy()
{
}

std::pair<float64_t,std::vector<index_t>> Dummy::run()
{
	std::vector<index_t> indices(cov.cols()-1);
	std::iota(indices.begin(), indices.end(), 0);
	return std::make_pair(0.0,indices);
}

void Dummy::set_params(Dummy::param_type params)
{
}
