/**********************************************************************************************************************
 *
 * @file console_logger.h
 *
 * @brief Declares the ConsoleLogger class that is a logging destination for the Logger class declared in logger.h
 *
 * @copyright Copyright (C) 2023 by Tom Hicks <headhunter3@gmail.com>
 *
 * Licensed under the MIT license see below for details.
 *
 **********************************************************************************************************************/
/*
 *  MIT License
 *
 * Copyright (c) 2023 Tom Hicks <headhunter3@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of
 * the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 **********************************************************************************************************************/
#ifndef CPPUtils__console_logger_h__
#define CPPUtils__console_logger_h__
#include <exception>
#include <functional>
#include <ostream>
#include <string>

#include "logger.h"

namespace CPPUtils {
/// @addtogroup logger Logger
/// @{

/// @brief A @link Logger::Destination @endlink that logs by writing to an std::ostream. By default this stream is
/// std::cout.
class ConsoleLogger : public Logger::Destination {
public:
  /// @brief Allocates any resources used by this destination. This sets the underlying output stream to std::cout.
  ConsoleLogger();

  /// @brief Frees any resources used by this destination. This is currently empty.
  virtual ~ConsoleLogger();

  /// @brief Logs a string message with a specific type.
  /// @param type The type of message to log.
  /// @param message The message to log.
  virtual void LogMessage(const Logger::MessageType &type, const std::string &message) const;

  /// @brief Logs an exception.
  /// @param type The type of message to log the exception as.
  /// @param ex The exception to log.
  virtual void LogError(const Logger::MessageType &type, const std::exception &ex) const;

  /// @brief Logs an exception with a message.
  /// @param type The type of message to log.
  /// @param message The message to log.
  /// @param ex The exception to log.
  virtual void LogError(const Logger::MessageType &type, const std::string &message, const std::exception &ex) const;

  /// @brief Sets the output stream used for logging.
  /// @param os The std::ostream to use for logging.
  virtual void SetOutputStream(std::ostream &os);

  /// @brief Gets the current output stream.
  /// @return The std::ostream used for logging.
  virtual std::ostream &GetOutputStream();

private:
  /// @brief Gets the title to be used based on the @link MessageType @endlink.
  /// @param type The @link Logger::MessageType @endlink to get the title for.
  /// @return The title to be used for this @link MessageType @endlink.
  std::string GetTitle(const Logger::MessageType &type) const;

  std::reference_wrapper<std::ostream> os_;
};

/// @}
} // namespace CPPUtils
#endif // CPPUtils__console_logger_h__
