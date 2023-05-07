/**********************************************************************************************************************
 *                                                                                                                    *
 * @file logger.h                                                                                                     *
 *                                                                                                                    *
 * @brief Declares the Logger class used as a generic interface to log to logging destinations.                       *
 * Multiple logging destinations can be registered with different thresholds.                                         *
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
#ifndef CPPUtils__Logger_h__
#define CPPUtils__Logger_h__

#include <exception>
#include <memory>
#include <string>
#include <vector>

namespace CPPUtils {
class Logger {
 public:
  enum MessageType { Unknown = 0, Debug, Verbose, Info, Warning, Error, Wtf };

  class Destination {
   public:
    Destination();
    virtual ~Destination();
    virtual void LogMessage(const MessageType& type, const std::string& message) const = 0;
    virtual void LogError(const MessageType& type, const std::exception& ex) const = 0;
    virtual void LogError(const MessageType& type, const std::string& message, const std::exception& ex) const = 0;
    virtual void SetMinType(const MessageType& type);
    virtual void SetMaxType(const MessageType& type);
    virtual MessageType GetMinType() const;
    virtual MessageType GetMaxType() const;

   protected:
    MessageType min_type_;
    MessageType max_type_;
  };

  virtual ~Logger();
  static std::shared_ptr<Logger> GetShared();
  static std::shared_ptr<Logger> GetUnique();
  virtual void AddDestination(std::shared_ptr<Logger::Destination> destination);
  virtual void ClearDestinations();

  // This one is special and annoying because it requires macros until the minimum standard is c++20.
#if __cplusplus >= 202002L
  void LogUnimplementedMethod(std::source_location = std::source_location::current());
#else
  void LogUnimplementedMethodReal(std::string method_name);
#endif

  void LogUnhandledError(const std::exception& ex);
  void LogUnimplementedFeature(const std::string& feature);
  void LogWtf(const std::string& message);
  void LogWtf(const std::exception& ex);
  void LogWtf(const std::string& message, const std::exception& ex);
  void LogError(const std::string& message);
  void LogError(const std::exception& ex);
  void LogError(const std::string& message, const std::exception& ex);
  void LogWarning(const std::string& message);
  void LogWarning(const std::exception& ex);
  void LogWarning(const std::string& message, const std::exception& ex);
  void LogInfo(const std::string& message);
  void LogInfo(const std::exception& ex);
  void LogInfo(const std::string& message, const std::exception& ex);
  void LogDebug(const std::string& message);
  void LogDebug(const std::exception& ex);
  void LogDebug(const std::string& message, const std::exception& ex);
  void LogVerbose(const std::string& message);
  void LogVerbose(const std::exception& ex);
  void LogVerbose(const std::string& message, const std::exception& ex);
  void Log(const MessageType& type, const std::string& message);
  void Log(const MessageType& type, const std::exception& ex);
  void Log(const MessageType& type, const std::string& message, const std::exception& ex);
  void LogToDo(const std::string& message);

 protected:
  Logger();

 private:
  std::vector<std::shared_ptr<Logger::Destination>> destinations_;
};
}  // End namespace CPPUtils

#if __cplusplus >= 202002L
#elif defined __GNUC__
#define LogUnimplementedMethod Logger::LogUnimplementedMethodReal(__PRETTY_FUNCTION__);
#else
#define LogUnimplementedMethod Logger::LogUnimplementedMethodReal(__FUNCTION__);
#endif

#endif  // End !defined(CPPUtils__Logger_h__)
