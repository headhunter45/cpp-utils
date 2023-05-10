/**********************************************************************************************************************
 *
 * @file windows_logger.h
 *
 * @brief Declares the WindowsLogger logging destination class declared in windows_logger.h.
 *
 * This logging destination works with the Logger class declared in logger.h and logs messages by creating a modal
 * windows message box
 *
 * @copyright Copyright (C) 2023 by Tom Hicks <headhunter3@gmail.com>
 *
 **********************************************************************************************************************/
/*
 * Licensed under the MIT license see below for details.
 *
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
#ifndef CPPUtils__windows_logger_h__
#define CPPUtils__windows_logger_h__
#ifdef _WIN32
#include <windows.h>

#include <cstdint>
#include <exception>
#include <string>

#include "logger.h"

namespace CPPUtils {
/// @addtogroup logger Logger
/// @{

/// @brief A @link Logger::Destination @endlink that logs by showing Windows message boxes.
class WindowsLogger : public CPPUtils::Logger {
public:
  /// @brief Allocates resources used by the WindowsLogger. This is currently empty.
  WindowsLogger();

  /// @brief Frees resources used by the WindowsLogger. This is currently empty.
  virtual ~WindowsLogger();

  /// @brief Logs a string message with a specific type.
  /// @param type The type of message to log.
  /// @param message The message to log.
  virtual void LogMessage(const MessageType &type, const std::string &message) const;

  /// @brief Logs an exception.
  /// @param type The type of message to log the exception as.
  /// @param ex The exception to log.
  virtual void LogError(const MessageType &type, const std::exception &ex) const;

  /// @brief Logs an exception with a message.
  /// @param type The type of message to log.
  /// @param message The message to log.
  /// @param ex The exception to log.
  virtual void LogError(const MessageType &type, const std::string &message, const std::exception &ex) const;

protected:
  /// @brief Shows message boxes
  /// @param message The body of the message box.
  /// @param title The title of the message box.
  /// @param uType The uType flags for the message box. This takes a bitmask of the MB_ defines from windows.h to
  /// specify the buttons and icon to show and the behavior of the message box.
  virtual void ShowMessageBox(const std::string &message, const std::string &title, UINT uType) const;

private:
  /// @brief Gets the default icon to use based on the @link MessageType @endlink.
  /// @param type The @link MessageType @endlink to use.
  /// @return The icon bits.
  uint32_t GetIcon(const MessageType &type) const;

  /// @brief Gets the default title to use based on the @link MessageType @endlink.
  /// @param type The @link MessageType @endlink to use.
  /// @return The title to use for the message box.
  std::string GetTitle(const MessageType &type) const;
};

/// @}
} // namespace CPPUtils

#endif // End defined(_WIN32)
#endif // End !defined(CPPUtils__windows_logger_h__)
