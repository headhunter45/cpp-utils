/**********************************************************************************************************************
 *                                                                                                                    *
 * @file windows_logger.h                                                                                             *
 *                                                                                                                    *
 * @brief Declares the WindowsLogger logging destination class declared in windows_logger.h.                          *
 * This logging destination works with the Logger class declared in logger.h and logs messages by creating a modal    *
 * windows message box                                                                                                *
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
#ifndef CPPUtils__WindowsLogger_h__
#define CPPUtils__WindowsLogger_h__
#include <windows.h>

#include <cstdint>
#include <exception>
#include <string>

#include "logger.h"

namespace CPPUtils {
class WindowsLogger : public CPPUtils::Logger {
 public:
  WindowsLogger();
  virtual ~WindowsLogger();
  virtual void LogMessage(const MessageType& type, const std::string& message) const;
  virtual void LogError(const MessageType& type, const std::exception& ex) const;
  virtual void LogError(const MessageType& type, const std::string& message, const std::exception& ex) const;

 protected:
  virtual void ShowMessageBox(const std::string& message, const std::string& title, UINT uType) const;

 private:
  uint32_t GetIcon(const MessageType& type) const;
  std::string GetTitle(const MessageType& type) const;
};
}  // namespace CPPUtils

#endif  // End !defined(CPPUtils__WindowsLogger_h__)
