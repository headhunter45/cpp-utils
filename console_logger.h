/**********************************************************************************************************************
 *                                                                                                                    *
 * @file console_logger.h                                                                                             *
 *                                                                                                                    *
 * @brief Declares the ConsoleLogger class that is a logging destination for the Logger class declared in logger.h    *
 * @copyright Copyright (C) 2023 by Tom Hicks <headhunter3@gmail.com>                                                 *
 *                                                                                                                    *
 * Licensed under the MIT license see below for details.                                                              *
 *                                                                                                                    *
 *  MIT License                                                                                                       *
 *                                                                                                                    *
 * Copyright (c) 2023 Tom Hicks <headhunter3@gmail.com>                                                               *
 *                                                                                                                    *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated       *
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation    *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and   *
 * to permit persons to whom the Software is furnished to do so, subject to the following conditions:                 *
 *                                                                                                                    *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of   *
 * the Software.                                                                                                      *
 *                                                                                                                    *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO   *
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF          *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  *
 * IN THE SOFTWARE.                                                                                                   *
 *                                                                                                                    *
 **********************************************************************************************************************/
#ifndef CONSOLE_LOGGER_H__
#define CONSOLE_LOGGER_H__
#include <exception>
#include <functional>
#include <ostream>
#include <string>

#include "logger.h"

namespace CPPUtils {
class ConsoleLogger : public Logger::Destination {
 public:
  ConsoleLogger();
  virtual ~ConsoleLogger();
  virtual void LogMessage(const Logger::MessageType& type, const std::string& message) const;
  virtual void LogError(const Logger::MessageType& type, const std::exception& ex) const;
  virtual void LogError(const Logger::MessageType& type, const std::string& message, const std::exception& ex) const;
  virtual void SetOutputStream(std::ostream& os);
  virtual std::ostream& GetOutputStream();

 private:
  std::string GetTitle(const Logger::MessageType& type) const;
  std::reference_wrapper<std::ostream> os_;
};
}  // namespace CPPUtils

#endif  // CONSOLE_LOGGER_H__

/*
Destination();
virtual ~Destination();
virtual void LogMessage(const MessageType& type, const std::string& message) const = 0;
virtual void LogError(const MessageType& type, const std::exception& ex) const = 0;
virtual void LogError(const MessageType& type, const std::string& message, const std::exception& ex) const = 0;
virtual void SetMinType(const MessageType& type);
virtual void SetMaxType(const MessageType& type);
virtual MessageType GetMinType() const;
virtual MessageType GetMaxType() const;
*/
