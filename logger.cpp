/**********************************************************************************************************************
 *                                                                                                                    *
 * @file logger.cpp                                                                                                   *
 *                                                                                                                    *
 * @brief Defines the Logger class declared in logger.h.                                                              *
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
#include "logger.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#if __cplusplus >= 202002L
#include <source_location>
#include <sstream>

namespace CPPUtils {
namespace {
using std::ostringstream;
using std::source_location;
}  // End namespace
}  // End namespace CPPUtils
#endif

namespace CPPUtils {
namespace {
using std::exception;
using std::shared_ptr;
using std::string;
shared_ptr<Logger> shared_logger_ = nullptr;
}  // End namespace

Logger::Destination::Destination() {}

Logger::Destination::~Destination() {}

Logger::Logger() {}

Logger::~Logger() {}

shared_ptr<Logger> Logger::GetShared() {
  if (shared_logger_ == nullptr) {
    shared_logger_ = shared_ptr<Logger>(new Logger());
  }
  return shared_logger_;
}

shared_ptr<Logger> Logger::GetUnique() {
  return shared_ptr<Logger>(new Logger());
}

void Logger::AddDestination(shared_ptr<Logger::Destination> destination) {
  destinations_.push_back(destination);
}

void Logger::ClearDestinations() {
  destinations_.clear();
}

void Logger::Destination::SetMinType(const MessageType& type) {
  if (type >= MessageType::Unknown && type <= MessageType::Wtf) {
    min_type_ = type;
  } else {
    min_type_ = MessageType::Unknown;
  }
}

void Logger::Destination::SetMaxType(const MessageType& type) {
  if (type >= MessageType::Unknown && type <= MessageType::Wtf) {
    max_type_ = type;
  } else {
    max_type_ = MessageType::Unknown;
  }
}

Logger::MessageType Logger::Destination::GetMinType() const {
  return min_type_;
}

Logger::MessageType Logger::Destination::GetMaxType() const {
  return max_type_;
}

#if __cplusplus >= 202002L
void Logger::LogUnimplementedMethod(source_location location) {
  ostringstream error_message;
  error_message << "Unimplemented method: \"" << location.function_name() << "\" at " << location.file_name() << " ("
                << location.line() << ":" << location.column() << ")";
  LogDebug(error_message.str());
}
#else
void Logger::LogUnimplementedMethodReal(string method_name) {
  LogDebug("Unimplemented method: " + method_name);
}
#endif

void Logger::LogUnhandledError(const exception& ex) {
  LogDebug("Unhandled exception", ex);
}

void Logger::LogUnimplementedFeature(const string& feature) {
  LogDebug("Unimplemented feature: " + feature);
}

void Logger::LogWtf(const string& message) {
  Log(Wtf, message);
}

void Logger::LogWtf(const exception& ex) {
  Log(Wtf, ex);
}

void Logger::LogWtf(const string& message, const exception& ex) {
  Log(Wtf, message, ex);
}

void Logger::LogError(const string& message) {
  Log(Error, message);
}

void Logger::LogError(const exception& ex) {
  Log(Error, ex);
}

void Logger::LogError(const string& message, const exception& ex) {
  Log(Error, message, ex);
}

void Logger::LogWarning(const string& message) {
  Log(Warning, message);
}

void Logger::LogWarning(const exception& ex) {
  Log(Warning, ex);
}

void Logger::LogWarning(const string& message, const exception& ex) {
  Log(Warning, message, ex);
}

void Logger::LogInfo(const string& message) {
  Log(Info, message);
}

void Logger::LogInfo(const exception& ex) {
  Log(Info, ex);
}

void Logger::LogInfo(const string& message, const exception& ex) {
  Log(Info, message, ex);
}

void Logger::LogDebug(const string& message) {
  Log(Debug, message);
}

void Logger::LogDebug(const exception& ex) {
  Log(Debug, ex);
}

void Logger::LogDebug(const string& message, const exception& ex) {
  Log(Debug, message, ex);
}

void Logger::LogVerbose(const string& message) {
  Log(Verbose, message);
}

void Logger::LogVerbose(const exception& ex) {
  Log(Verbose, ex);
}

void Logger::LogVerbose(const string& message, const exception& ex) {
  Log(Verbose, message, ex);
}

void Logger::Log(const MessageType& type, const string& message) {
  for (const auto& destination : destinations_) {
    if (type >= destination->GetMinType() && type <= destination->GetMaxType()) {
      destination->LogMessage(type, message);
    }
  }
}

void Logger::Log(const MessageType& type, const exception& ex) {
  for (const auto& destination : destinations_) {
    if (type >= destination->GetMinType() && type <= destination->GetMaxType()) {
      destination->LogError(type, ex);
    }
  }
}

void Logger::Log(const MessageType& type, const string& message, const exception& ex) {
  for (const auto& destination : destinations_) {
    if (type >= destination->GetMinType() && type <= destination->GetMaxType()) {
      destination->LogError(type, message, ex);
    }
  }
}

void Logger::LogToDo(const string& message) {
  LogDebug("TODO: " + message);
}
}  // End namespace CPPUtils
