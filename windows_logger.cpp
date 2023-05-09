/**********************************************************************************************************************
 *                                                                                                                    *
 * @file windows_logger.cpp                                                                                           *
 *                                                                                                                    *
 * @brief Defines the WindowsLogger logging destination class declared in windows_logger.h.                           *
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
// Documentation for the windows api stuff used here is available at
// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
#include "windows_logger.h"
#ifdef _WIN32
#include <windows.h>
#include <winuser.h>

#include <cstdlib>
#include <exception>
#include <string>
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "winmm.lib")

namespace CPPUtils {
#ifdef _WIN32
namespace {
using std::exception;
using std::string;
const string kDebugMessageTitle = "Debug";
const string kErrorMessageTitle = "ERROR";
const string kInfoMessageTitle = "Information";
const string kVerboseMessageTitle = "Verbose";
const string kWarningMessageTitle = "Warning";
const string kWtfMessageTitle = "How did you let this happen?";
const string kUnknownMessageTitle = "Unclassified";
} // End namespace

WindowsLogger::WindowsLogger() {}

WindowsLogger::~WindowsLogger() {}

void WindowsLogger::LogMessage(const MessageType &type, const string &message) const {
  ShowMessageBox(message, GetTitle(type), MB_OK | GetIcon(type));
}

void WindowsLogger::LogError(const MessageType &type, const exception &ex) const {
  string body = (string) "Exception: " + ex.what();
  ShowMessageBox(body, GetTitle(type), MB_OK | GetIcon(type));
}

void WindowsLogger::LogError(const MessageType &type, const std::string &message, const std::exception &ex) const {
  string body = (string) "Exception: " + ex.what() + " with message " + message;
  ShowMessageBox(body, GetTitle(type), MB_OK | GetIcon(type));
}

void WindowsLogger::ShowMessageBox(const string &message, const string &title, UINT uType) const {
  MessageBox(NULL, message.c_str(), title.c_str(), uType);
}

/*
MB_ICONEXCLAMATION - An exclamation point.
MB_ICONWARNING     - An exclamation point. Alias for MB_ICONEXCLAMATION.
MB_ICONINFORMATION - A lowercase i.
MB_ICONASTERISK    - A lowercase i. Alias for MB_ICONINFORMATION.
MB_ICONQUESTION    - A question mark. Don't use this, because it's confusing to users.
MB_ICONSTOP        - A stop sign icon.
MB_ICONERROR       - A stop sign icon. Alias for MB_ICONSTOP.
MB_ICONHAND        - A stop sign icon. Alias for MB_ICONSTOP.
*/
uint32_t WindowsLogger::GetIcon(const MessageType &type) const {
  switch (type) {
  case MessageType::Debug:
    return MB_ICONEXCLAMATION;
  case MessageType::Error:
    return MB_ICONSTOP;
  case MessageType::Info:
    return MB_ICONINFORMATION;
  case MessageType::Verbose:
    return MB_ICONINFORMATION;
  case MessageType::Warning:
    return MB_ICONEXCLAMATION;
  case MessageType::Wtf:
    return MB_ICONSTOP;
  default:
    return MB_ICONINFORMATION;
  }
}

string WindowsLogger::GetTitle(const MessageType &type) const {
  switch (type) {
  case MessageType::Debug:
    return kDebugMessageTitle;
  case MessageType::Error:
    return kErrorMessageTitle;
  case MessageType::Info:
    return kInfoMessageTitle;
  case MessageType::Verbose:
    return kVerboseMessageTitle;
  case MessageType::Warning:
    return kWarningMessageTitle;
  case MessageType::Wtf:
    return kWtfMessageTitle;
  default:
    return kUnknownMessageTitle;
  }
}
#endif
} // End namespace CPPUtils

#ifdef WIN32
namespace CPPUtils {}; // namespace CPPUtils
#endif
