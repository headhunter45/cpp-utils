/**********************************************************************************************************************
 *                                                                                                                    *
 * @file console_logger_test.cpp                                                                                      *
 *                                                                                                                    *
 * @brief Defines tests for the ConsoleLogger class that is a logging destination for the Logger class declared in    *
 * logger.h.                                                                                                          *
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
#include <sstream>
#include <tuple>

#include "logger.h"
#include "tinytest.h"

namespace {
using CPPUtils::ConsoleLogger;
using CPPUtils::Logger;
using std::cout;
using std::make_shared;
using std::make_tuple;
using std::ostream;
using std::ostringstream;
using std::string;
using TinyTest::execute_suite;
using TinyTest::make_test;
using TinyTest::make_test_suite;
using TinyTest::TestResults;
string no_errors = "no errors";

}  // namespace

TestResults test_ConsoleLogger_LogMessage() {
  auto log_message = [](const Logger::MessageType& type, const string& message) -> string {
    ostringstream errors;
    ostringstream output;
    auto destination = make_shared<ConsoleLogger>();
    destination->SetOutputStream(output);
    destination->LogMessage(type, message);
    return output.str();
  };
  return execute_suite(make_test_suite("CPPUtils::ConsoleLogger::LogMessage(cosnt std::string&)",
                                       log_message,
                                       {make_test("should print \"[Debug] this is a message\"",
                                                  (string) "[Debug] this is a message\n",
                                                  make_tuple(Logger::MessageType::Debug, "this is a message")),
                                        make_test("should print \"[Error] this is an error\"",
                                                  (string) "[Error] this is an error\n",
                                                  make_tuple(Logger::MessageType::Error, "this is an error")),
                                        make_test("should print \"[Wtf] what a terrible failure\"",
                                                  (string) "[Wtf] what a terrible failure\n",
                                                  make_tuple(Logger::MessageType::Wtf, "what a terrible failure")),
                                        make_test("should print \"[Info] this is some information\"",
                                                  (string) "[Info] this is some information\n",
                                                  make_tuple(Logger::MessageType::Info, "this is some information")),
                                        make_test("should print \"[Warning] this is a warning\"",
                                                  (string) "[Warning] this is a warning\n",
                                                  make_tuple(Logger::MessageType::Warning, "this is a warning")),
                                        make_test("should print \"[Verbose] this is verbose\"",
                                                  (string) "[Verbose] this is verbose\n",
                                                  make_tuple(Logger::MessageType::Verbose, "this is verbose")),
                                        make_test("should print \"[Unclassified] unclassified message\"",
                                                  (string) "[Unclassified] unclassified message\n",
                                                  make_tuple((Logger::MessageType)1000, "unclassified message"))}));
}

TestResults test_ConsoleLogger_LogErrorWithMessage() {
  auto log_error = [](const Logger::MessageType& type, const std::exception& ex) -> string {
    ostringstream errors;
    ostringstream output;
    auto destination = make_shared<ConsoleLogger>();
    destination->SetOutputStream(output);
    destination->LogError(type, ex);
    return output.str();
  };
  return execute_suite(
      make_test_suite("CPPUtils::ConsoleLogger::LogError(const std::exception&)",
                      log_error,
                      {make_test("should print \"[Debug] caught exception: this is an exception\"",
                                 (string) "[Debug] caught exception: this is an exception\n",
                                 make_tuple(Logger::MessageType::Debug, std::runtime_error("this is an exception"))),
                       make_test("should print \"[Error] caught exception: this is an error\"",
                                 (string) "[Error] caught exception: this is an error\n",
                                 make_tuple(Logger::MessageType::Error, std::runtime_error("this is an error"))),
                       make_test("should print \"[Wtf] caught exception: what a terrible failure\"",
                                 (string) "[Wtf] caught exception: what a terrible failure\n",
                                 make_tuple(Logger::MessageType::Wtf, std::runtime_error("what a terrible failure"))),
                       make_test("should print \"[Info] caught exception: this is some information\"",
                                 (string) "[Info] caught exception: this is some information\n",
                                 make_tuple(Logger::MessageType::Info, std::runtime_error("this is some information"))),
                       make_test("should print \"[Warning] caught exception: this is a warning\"",
                                 (string) "[Warning] caught exception: this is a warning\n",
                                 make_tuple(Logger::MessageType::Warning, std::runtime_error("this is a warning"))),
                       make_test("should print \"[Verbose] caught exception: this is verbose\"",
                                 (string) "[Verbose] caught exception: this is verbose\n",
                                 make_tuple(Logger::MessageType::Verbose, std::runtime_error("this is verbose"))),
                       make_test("should print \"[Unclassified] caught exception: unclassified message\"",
                                 (string) "[Unclassified] caught exception: unclassified message\n",
                                 make_tuple((Logger::MessageType)1000, std::runtime_error("unclassified message")))}));
}

TestResults test_ConsoleLogger_LogErrorWithoutMessage() {
  auto log_error = [](const Logger::MessageType& type, const std::string& message, const std::exception& ex) -> string {
    ostringstream errors;
    ostringstream output;
    auto destination = make_shared<ConsoleLogger>();
    destination->SetOutputStream(output);
    destination->LogError(type, message, ex);
    return output.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::ConsoleLogger::LogError(const std::string&, const std::exception&)",
      log_error,
      {
          make_test(
              "should print \"[Debug] this is a message with caught exception this is an exception\"",
              (string) "[Debug] this is a message with caught exception this is an exception\n",
              make_tuple(Logger::MessageType::Debug, "this is a message", std::runtime_error("this is an exception"))),
          make_test(
              "should print \"[Error] this is an error with caught exception this is an exception\"",
              (string) "[Error] this is an error with caught exception this is an exception\n",
              make_tuple(Logger::MessageType::Error, "this is an error", std::runtime_error("this is an exception"))),
          make_test(
              "should print \"[Wtf] what a terrible failure with caught exception this is an exception\"",
              (string) "[Wtf] what a terrible failure with caught exception this is an exception\n",
              make_tuple(
                  Logger::MessageType::Wtf, "what a terrible failure", std::runtime_error("this is an exception"))),
          make_test(
              "should print \"[Info] this is some information with caught exception this is an exception\"",
              (string) "[Info] this is some information with caught exception this is an exception\n",
              make_tuple(
                  Logger::MessageType::Info, "this is some information", std::runtime_error("this is an exception"))),
          make_test("should print \"[Warning] this is a warning with caught exception this is an exception\"",
                    (string) "[Warning] this is a warning with caught exception this is an exception\n",
                    make_tuple(
                        Logger::MessageType::Warning, "this is a warning", std::runtime_error("this is an exception"))),
          make_test(
              "should print \"[Verbose] this is verbose with caught exception this is an exception\"",
              (string) "[Verbose] this is verbose with caught exception this is an exception\n",
              make_tuple(Logger::MessageType::Verbose, "this is verbose", std::runtime_error("this is an exception"))),
          make_test("should print \"[Unclassified] unclassified message with caught exception this is an exception\"",
                    (string) "[Unclassified] unclassified message with caught exception this is an exception\n",
                    make_tuple(
                        (Logger::MessageType)1000, "unclassified message", std::runtime_error("this is an exception"))),
      }));
}

TestResults test_ConsoleLogger_SetOutputStream() {
  auto set_get_output_stream = [](std::ostream& os, bool only_get) -> bool {
    auto destination = make_shared<ConsoleLogger>();
    if (!only_get) {
      destination->SetOutputStream(os);
    }
    return &destination->GetOutputStream() == &os;
  };
  std::ostringstream os1;
  return execute_suite(make_test_suite(
      "CPPUtils::ConsoleLogger::Set/GetOutputStream(std::ostream&)",
      set_get_output_stream,
      {
          make_test("should get cout by default", true, make_tuple(std::ref(std::cout), true)),
          make_test("should set and get the same output stream", true, make_tuple(std::ref(std::cout), false)),
          make_test("should set and get the same output stream", true, make_tuple(std::ref((std::ostream&)os1), false)),
      }));
}

int main(int argc, char* argv[]) {
  TestResults results;

  results += test_ConsoleLogger_LogMessage();
  results += test_ConsoleLogger_LogErrorWithMessage();
  results += test_ConsoleLogger_LogErrorWithoutMessage();
  results += test_ConsoleLogger_SetOutputStream();

  PrintResults(cout, results);

  return results.failed() + results.errors();
}
