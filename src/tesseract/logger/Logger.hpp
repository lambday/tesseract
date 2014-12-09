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

#ifndef LOGGER_H__
#define LOGGER_H__

#include <tesseract/base/types.h>

namespace tesseract
{

/** log level */
enum LogLevel
{
	// no log messasges (default)
	None,
	// generic information to the user
	Info,
	// warning
	Warning,
	// error
	Error,
	// function entry/exit, computed values
	Debug,
	// memory location, mem alloc/dealloc
	MemDebug
};

/** @brief class Logger for logging purpose. Outputs the logs in stdout */
class Logger
{
public:
	/** default constructor */
	Logger();

	/** @param log_level desired loglevel */
	void set_loglevel(LogLevel log_level);

	/** @param log_level desired loglevel */
	const LogLevel get_loglevel() const;

	/** prints the log if specified message's loglevel is less that the loglevel specified
	 * @param level the log level of the message
	 * @param format the format of the log message
	 */
	void write(LogLevel level, const char* format, ...);

	/** @param vector to be printed in stdout */
	void print_vector(std::vector<index_t> inds);

	/** @param matrix to be printed in stdout */
	void print_matrix(const Eigen::Ref<const Matrix<float64_t>>& mat);

private:
	/** log level */
	LogLevel loglevel;
};

}

#endif // LOGGER_H__
