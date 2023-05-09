/**********************************************************************************************************************
 *                                                                                                                    *
 * @file windows_logger_test.cpp                                                                                      *
 *                                                                                                                    *
 * @brief Defines tests for the WindowsLogger logging destination class declared in windows_logger.h.                 *
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
#include "windows_logger.h"

#include <iostream>
#include <sstream>
#include <tuple>
#include <winuser.h>

#include "logger.h"
#include "tinytest.h"

namespace {
using CPPUtils::Logger;
using CPPUtils::WindowsLogger;
using std::cout;
using std::endl;
using std::get;
using std::hex;
using std::make_shared;
using std::make_tuple;
using std::ostream;
using std::ostringstream;
using std::shared_ptr;
using std::string;
using std::tuple;
using std::vector;
using TinyTest::execute_suite;
using TinyTest::make_test;
using TinyTest::make_test_suite;
using TinyTest::TestResults;
string no_errors = "no errors";
typedef tuple<string, string, UINT> MessageBoxEvent;

class WindowsLoggerSpy : public WindowsLogger {
public:
  mutable vector<MessageBoxEvent> log;

protected:
  virtual void ShowMessageBox(const string &body, const string &title, UINT u_type) const override {
    log.push_back(make_tuple(body, title, u_type));
  }
};

void ExpectLogSize(ostream &errors, size_t expected, const shared_ptr<WindowsLoggerSpy> &spy) {
  size_t actual = spy->log.size();
  if (actual != expected) {
    errors << "Log size mismatch. Expected: " << expected << ", Actual: " << actual << endl;
  }
}

void ExpectMessage(ostream &errors, const string &expected, const MessageBoxEvent &event) {
  string actual = get<0>(event);
  if (actual != expected) {
    errors << "Message mismatch. Expected: " << expected << ", Actual: " << actual << endl;
  }
}

void ExpectTitle(ostream &errors, const string &expected, const MessageBoxEvent &event) {
  string actual = get<1>(event);
  if (actual != expected) {
    errors << "Title mismatch. Expected: \"" << expected << "\", Actual: \"" << actual << "\"" << endl;
  }
}

string Join(vector<string> parts, const string &separator) {
  ostringstream os;

  bool first = true;
  ;
  for_each(parts.begin(), parts.end(), [&os, &first, separator](string part) {
    if (first) {
      first = false;
    } else {
      os << separator;
    }
    os << part;
  });
  return os.str();
}

string ConvertUTypeToString(UINT u_type, bool ignore_defaults = true) {
  UINT u_button = u_type & MB_TYPEMASK;
  UINT u_icon = u_type & MB_ICONMASK;
  UINT u_default_button = u_type & MB_DEFMASK;
  UINT u_mode = u_type & MB_MODEMASK;
  UINT u_misc = u_type & MB_MISCMASK;

  vector<string> parts;

  // MB_OK = 0x00000000L
  if (u_button == MB_OK) {
    parts.push_back("MB_OK");
  }
  // MB_OKCANCEL = 0x00000001L
  if (u_button == MB_OKCANCEL) {
    parts.push_back("MB_OKCANCEL");
  }
  // MB_ABORTRETRYIGNORE = 0x00000002L
  if (u_button == MB_ABORTRETRYIGNORE) {
    parts.push_back("MB_ABORTRETRYIGNORE");
  }
  // MB_YESNOCANCEL = 0x00000003L
  if (u_button == MB_YESNOCANCEL) {
    parts.push_back("MB_YESNOCANCEL");
  }
  // MB_YESNO = 0x00000004L
  if (u_button == MB_YESNO) {
    parts.push_back("MB_YESNO");
  }
  // MB_RETRYCANCEL = 0x00000005L
  if (u_button == MB_RETRYCANCEL) {
    parts.push_back("MB_RETRYCANCEL");
  }
  // MB_CANCELTRYCONTINUE = 0x00000006L
  if (u_button == MB_CANCELTRYCONTINUE) {
    parts.push_back("MB_CANCELTRYCONTINUE");
  }

  // MB_ICONHAND = 0x00000010L
  if (u_icon == MB_ICONHAND) {
    parts.push_back("MB_ICONHAND");
  }
  // MB_ICONQUESTION = 0x00000020L
  if (u_icon == MB_ICONQUESTION) {
    parts.push_back("MB_ICONQUESTION");
  }
  // MB_ICONEXCLAMATION = 0x00000030L
  if (u_icon == MB_ICONEXCLAMATION) {
    parts.push_back("MB_ICONEXCLAMATION");
  }
  // MB_ICONASTERISK = 0x00000040L
  if (u_icon == MB_ICONASTERISK) {
    parts.push_back("MB_ICONASTERISK");
  }
  // MB_USERICON = 0x00000080L
  if (u_icon == MB_USERICON) {
    parts.push_back("MB_USERICON");
  }

  // MB_DEFBUTTON1 = 0x00000000L
  if (u_default_button == MB_DEFBUTTON1 && !ignore_defaults) {
    parts.push_back("MB_DEFBUTTON1");
  }
  // MB_DEFBUTTON2 = 0x00000100L
  if (u_default_button == MB_DEFBUTTON2) {
    parts.push_back("MB_DEFBUTTON2");
  }
  // MB_DEFBUTTON3 = 0x00000200L
  if (u_default_button == MB_DEFBUTTON3) {
    parts.push_back("MB_DEFBUTTON3");
  }
  // MB_DEFBUTTON4 = 0x00000300L
  if (u_default_button == MB_DEFBUTTON4) {
    parts.push_back("MB_DEFBUTTON4");
  }

  // MB_APPLMODAL = 0x00000000L
  if (u_mode == MB_APPLMODAL && !ignore_defaults) {
    parts.push_back("MB_APPLMODAL");
  }
  // MB_SYSTEMMODAL = 0x00001000L
  if (u_mode == MB_SYSTEMMODAL) {
    parts.push_back("MB_SYSTEMMODAL");
  }
  // MB_TASKMODAL = 0x00002000L
  if (u_mode == MB_TASKMODAL) {
    parts.push_back("MB_TASKMODAL");
  }
  // MB_HELP = 0x00004000L
  if (u_mode == MB_HELP) {
    parts.push_back("MB_HELP");
  }
  // MB_NOFOCUS = 0x00008000L
  if (u_mode == MB_NOFOCUS) {
    parts.push_back("MB_NOFOCUS");
  }

  // MB_SETFOREGROUND = 0x00010000L
  if (u_misc == MB_SETFOREGROUND) {
    parts.push_back("MB_SETFOREGROUND");
  }
  // MB_DEFAULT_DESKTOP_ONLY = 0x00020000L
  if (u_misc == MB_DEFAULT_DESKTOP_ONLY) {
    parts.push_back("MB_DEFAULT_DESKTOP_ONLY");
  }
  // MB_TOPMOST = 0x00040000L
  if (u_misc == MB_TOPMOST) {
    parts.push_back("MB_TOPMOST");
  }
  // MB_RIGHT = 0x00080000L
  if (u_misc == MB_RIGHT) {
    parts.push_back("MB_RIGHT");
  }

  // MB_RTLREADING = 0x00100000L
  if (u_misc == MB_RTLREADING) {
    parts.push_back("MB_RTLREADING");
  }
  // MB_SERVICE_NOTIFICATION = 0x00200000L
  // MB_SERVICE_NOTIFICATION = 0x00040000L
  if ((u_misc == MB_SERVICE_NOTIFICATION) || (u_misc == MB_SERVICE_NOTIFICATION)) {
    parts.push_back("MB_SERVICE_NOTIFICATION");
  }

  return Join(parts, " | ");
}

void ExpectUType(ostream &errors, UINT expected, const MessageBoxEvent &event) {
  UINT actual = get<2>(event);
  if (actual != expected) {
    errors << "u_type mismatch. Expected: " << hex << expected << " (" << ConvertUTypeToString(expected)
           << "), Actual: " << hex << actual << " (" << ConvertUTypeToString(actual) << ")" << endl;
  }
}

string GetErrors(ostringstream &error_messages) {
  string errors = error_messages.str();
  if (errors.size() > 0) {
    return errors;
  }
  return no_errors;
}
} // namespace

TestResults test_WindowsLogger_LogMessage() {
  auto log_message = [](const Logger::MessageType &type,
                        const string &message,
                        const string &body,
                        const string &title,
                        UINT u_type) -> string {
    ostringstream errors;
    auto destination = make_shared<WindowsLoggerSpy>();
    destination->LogMessage(type, message);
    ExpectLogSize(errors, 1, destination);
    if (destination->log.size() > 0) {
      auto event = destination->log.at(0);
      ExpectMessage(errors, body, event);
      ExpectTitle(errors, title, event);
      ExpectUType(errors, u_type, event);
    }
    return GetErrors(errors);
  };
  return execute_suite(make_test_suite(
      "CPPUtils::WindowsLogger::LogMessage(cosnt std::string&)",
      log_message,
      {
          make_test("should log \"this is a message\" with title \"Debug\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Debug,
                               "this is a message",
                               "this is a message",
                               "Debug",
                               MB_OK | MB_ICONEXCLAMATION)),
          make_test(
              "should log \"this is an error\" with title \"ERROR\"",
              no_errors,
              make_tuple(
                  Logger::MessageType::Error, "this is an error", "this is an error", "ERROR", MB_OK | MB_ICONHAND)),
          make_test("should log \"what a terrible failure\" with title \"How did you let this happen?\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Wtf,
                               "what a terrible failure",
                               "what a terrible failure",
                               "How did you let this happen?",
                               MB_OK | MB_ICONHAND)),
          make_test("should log \"this is some information\" with title \"Information\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Info,
                               "this is some information",
                               "this is some information",
                               "Information",
                               MB_OK | MB_ICONASTERISK)),
          make_test("should log \"this is a warning\" with title \"Warning\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Warning,
                               "this is a warning",
                               "this is a warning",
                               "Warning",
                               MB_OK | MB_ICONEXCLAMATION)),
          make_test("should log \"this is verbose\" with title \"Verbose\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Verbose,
                               "this is verbose",
                               "this is verbose",
                               "Verbose",
                               MB_OK | MB_ICONASTERISK)),
          make_test("should log \"unclassified message\" with title \"Unclassified\"",
                    no_errors,
                    make_tuple((Logger::MessageType)1000,
                               "unclassified message",
                               "unclassified message",
                               "Unclassified",
                               MB_OK | MB_ICONASTERISK)),
      }));
}

TestResults test_WindowsLogger_LogErrorWithMessage() {
  auto log_error = [](const Logger::MessageType &type,
                      const std::exception &ex,
                      const string &expected_body,
                      const string &expected_title,
                      UINT expected_utype) -> string {
    ostringstream errors;
    auto destination = make_shared<WindowsLoggerSpy>();
    destination->LogError(type, ex);
    ExpectLogSize(errors, 1, destination);
    if (destination->log.size() > 0) {
      auto event = destination->log.at(0);
      ExpectMessage(errors, expected_body, event);
      ExpectTitle(errors, expected_title, event);
      ExpectUType(errors, expected_utype, event);
    }
    return GetErrors(errors);
  };
  return execute_suite(make_test_suite(
      "CPPUtils::WindowsLogger::LogError(const std::exception&)",
      log_error,
      {
          make_test("should log \"Exception: this is an exception\" with title \"Debug\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Debug,
                               std::runtime_error("this is an exception"),
                               "Exception: this is an exception",
                               "Debug",
                               MB_OK | MB_ICONEXCLAMATION)),
          make_test("should log \"Exception: this is an error\" with title \"ERROR\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Error,
                               std::runtime_error("this is an error"),
                               "Exception: this is an error",
                               "ERROR",
                               MB_OK | MB_ICONHAND)),
          make_test("should log \"Exception: what a terrible failure\" with title \"How did you let this happen?\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Wtf,
                               std::runtime_error("what a terrible failure"),
                               "Exception: what a terrible failure",
                               "How did you let this happen?",
                               MB_OK | MB_ICONHAND)),
          make_test("should log \"Exception: this is some information\" with title \"Information\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Info,
                               std::runtime_error("this is some information"),
                               "Exception: this is some information",
                               "Information",
                               MB_OK | MB_ICONASTERISK)),
          make_test("should log \"Exception: this is a warning\" with title \"Warning\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Warning,
                               std::runtime_error("this is a warning"),
                               "Exception: this is a warning",
                               "Warning",
                               MB_OK | MB_ICONEXCLAMATION)),
          make_test("should log \"Exception: this is verbose\" with title \"Verbose\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Verbose,
                               std::runtime_error("this is verbose"),
                               "Exception: this is verbose",
                               "Verbose",
                               MB_OK | MB_ICONASTERISK)),
          make_test("should log \"Exception: unclassified message\" with title \"Unclassified\"",
                    no_errors,
                    make_tuple((Logger::MessageType)1000,
                               std::runtime_error("unclassified message"),
                               "Exception: unclassified message",
                               "Unclassified",
                               MB_OK | MB_ICONASTERISK)),
      }));
}

TestResults test_WindowsLogger_LogErrorWithoutMessage() {
  auto log_error = [](const Logger::MessageType &type,
                      const std::string &message,
                      const std::exception &ex,
                      const string &expected_body,
                      const string &expected_title,
                      UINT expected_utype) -> string {
    ostringstream errors;
    auto destination = make_shared<WindowsLoggerSpy>();
    destination->LogError(type, ex);
    ExpectLogSize(errors, 1, destination);
    if (destination->log.size() > 0) {
      auto event = destination->log.at(0);
      ExpectMessage(errors, expected_body, event);
      ExpectTitle(errors, expected_title, event);
      ExpectUType(errors, expected_utype, event);
    }
    return GetErrors(errors);
  };
  return execute_suite(make_test_suite(
      "CPPUtils::WindowsLogger::LogError(const std::string&, const std::exception&)",
      log_error,
      {
          make_test("should log \"Exception: this is an exception\" with title \"Debug\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Debug,
                               "this is a message",
                               std::runtime_error("this is an exception"),
                               "Exception: this is an exception",
                               "Debug",
                               MB_OK | MB_ICONEXCLAMATION)),
          make_test("should log \"Exception: this is an exception\" with title \"ERROR\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Error,
                               "this is an error",
                               std::runtime_error("this is an exception"),
                               "Exception: this is an exception",
                               "ERROR",
                               MB_OK | MB_ICONHAND)),
          make_test("should log \"Exception: this is an exception\" with title \"How did you let this happen?\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Wtf,
                               "what a terrible failure",
                               std::runtime_error("this is an exception"),
                               "Exception: this is an exception",
                               "How did you let this happen?",
                               MB_OK | MB_ICONHAND)),
          make_test("should log \"Exception this is an exception\" with title \"Information\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Info,
                               "this is some information",
                               std::runtime_error("this is an exception"),
                               "Exception: this is an exception",
                               "Information",
                               MB_OK | MB_ICONASTERISK)),
          make_test("should log \"Exception: this is a warning with caught exception this is an exception\" with title "
                    "\"Warning\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Warning,
                               "this is a warning",
                               std::runtime_error("this is an exception"),
                               "Exception: this is an exception",
                               "Warning",
                               MB_OK | MB_ICONEXCLAMATION)),
          make_test("should log \"Exception: this is an exception\" with title \"Verbose\"",
                    no_errors,
                    make_tuple(Logger::MessageType::Verbose,
                               "this is verbose",
                               std::runtime_error("this is an exception"),
                               "Exception: this is an exception",
                               "Verbose",
                               MB_OK | MB_ICONASTERISK)),
          make_test("should log \"Exception: this is an exception\" with title \"Unclassified\"",
                    no_errors,
                    make_tuple((Logger::MessageType)1000,
                               "unclassified message",
                               std::runtime_error("this is an exception"),
                               "Exception: this is an exception",
                               "Unclassified",
                               MB_OK | MB_ICONASTERISK)),
      }));
}

int main(int argc, char *argv[]) {
  TestResults results;

  results += test_WindowsLogger_LogMessage();
  results += test_WindowsLogger_LogErrorWithMessage();
  results += test_WindowsLogger_LogErrorWithoutMessage();

  PrintResults(cout, results);

  return results.failed() + results.errors();
}
