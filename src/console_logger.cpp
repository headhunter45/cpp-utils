/**********************************************************************************************************************
 *                                                                                                                    *
 * @file console_logger.cpp                                                                                           *
 *                                                                                                                    *
 * @brief Defines the ConsoleLogger class that is a logging destination for the Logger class declared in logger.h     *
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
#include "console_logger.h"

#include <iostream>
#include <string>

namespace CPPUtils {
namespace {
using std::endl;
using std::string;
}  // End namespace

ConsoleLogger::ConsoleLogger() : os_(std::cout) {}

ConsoleLogger::~ConsoleLogger() {}

void ConsoleLogger::LogMessage(const Logger::MessageType& type, const std::string& message) const {
  os_.get() << "[" << GetTitle(type) << "] " << message << endl;
}

void ConsoleLogger::LogError(const Logger::MessageType& type, const std::exception& ex) const {
  os_.get() << "[" << GetTitle(type) << "] caught exception: " << ex.what() << endl;
}

void ConsoleLogger::LogError(const Logger::MessageType& type,
                             const std::string& message,
                             const std::exception& ex) const {
  os_.get() << "[" << GetTitle(type) << "] " << message << " with caught exception " << ex.what() << endl;
}

string ConsoleLogger::GetTitle(const Logger::MessageType& type) const {
  switch (type) {
    case Logger::MessageType::Debug:
      return "Debug";
    case Logger::MessageType::Error:
      return "Error";
    case Logger::MessageType::Wtf:
      return "Wtf";
    case Logger::MessageType::Info:
      return "Info";
    case Logger::MessageType::Warning:
      return "Warning";
    case Logger::MessageType::Verbose:
      return "Verbose";
    default:
      return "Unclassified";
  }
}

void ConsoleLogger::SetOutputStream(std::ostream& os) {
  os_ = os;
}

std::ostream& ConsoleLogger::GetOutputStream() {
  return os_;
}

}  // End namespace CPPUtils
