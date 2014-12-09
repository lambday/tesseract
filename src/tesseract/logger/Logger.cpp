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

#include <tesseract/logger/Logger.hpp>
#include <cstdio>
#include <cstdarg>
#include <algorithm>
#include <iostream>

using namespace tesseract;

Logger::Logger() : loglevel(None)
{
}

void Logger::set_loglevel(LogLevel log_level)
{
	loglevel = log_level;
}

const LogLevel Logger::get_loglevel() const
{
	return loglevel;
}

void Logger::write(LogLevel level, const char* format, ...)
{
	if (level <= loglevel)
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

void Logger::print_vector(std::vector<index_t> inds)
{
	printf("vector = [");
	std::for_each(inds.begin(), inds.end(), [](index_t val) { printf("%u ", val); });
	printf("]\n");
}

void Logger::print_matrix(const Eigen::Ref<const Matrix<float64_t>>& mat)
{
	std::cout << "matrix = " << std::endl << mat << std::endl;
}
