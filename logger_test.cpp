/**********************************************************************************************************************
 *                                                                                                                    *
 * @file logger_test.h                                                                                                *
 *                                                                                                                    *
 * @brief Defines tests for the Logger class declared in logger.h.                                                    *
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

#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "tinytest.h"

namespace {
using CPPUtils::Logger;
using std::cout;
using std::endl;
using std::get;
using std::make_optional;
using std::make_shared;
using std::make_tuple;
using std::nullopt;
using std::optional;
using std::ostream;
using std::ostringstream;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::tuple;
using std::vector;
using TinyTest::execute_suite;
using TinyTest::make_test;
using TinyTest::make_test_suite;
using TinyTest::TestResults;

string no_errors = "no errors";
typedef tuple<Logger::MessageType, optional<string>, optional<string>> LogEntry;

class SpyLoggerDestination : public Logger::Destination {
 public:
  SpyLoggerDestination() {
    min_type_ = Logger::MessageType::Unknown;
    max_type_ = Logger::MessageType::Wtf;
  }

  virtual ~SpyLoggerDestination() {}

  virtual void LogMessage(const Logger::MessageType& type, const std::string& message) const {
    log.push_back(make_tuple(type, message, nullopt));
  }

  virtual void LogError(const Logger::MessageType& type, const std::exception& ex) const {
    log.push_back(make_tuple(type, nullopt, ex.what()));
  }

  virtual void LogError(const Logger::MessageType& type, const std::string& message, const std::exception& ex) const {
    log.push_back(make_tuple(type, message, ex.what()));
  }

  mutable vector<LogEntry> log;
};

template <typename T>
ostream& PrintOptional(ostream& os, optional<T> op) {
  if (op.has_value()) {
    os << op.value();
  } else {
    os << "nullopt";
  }
  return os;
}

void ExpectMessageType(ostream& error_message, const Logger::MessageType& expected, const LogEntry& log_entry) {
  Logger::MessageType actual = get<0>(log_entry);
  if (actual != expected) {
    error_message << "Unexpected MessageType expected: " << expected << ", actual: " << actual << endl;
  }
}

void ExpectMessageText(ostream& error_message, optional<string> expected, const LogEntry& log_entry) {
  optional<string> actual = get<1>(log_entry);
  if (actual != expected) {
    error_message << "Unexpected message text. Expected: ";
    PrintOptional(error_message, expected) << ", Actual: ";
    PrintOptional(error_message, actual) << endl;
  }
}

void ExpectMessageException(ostream& error_message, optional<string> expected, const LogEntry& log_entry) {
  optional<string> actual = get<2>(log_entry);
  if (actual != expected) {
    error_message << "Unexpected message exception. Expected: ";
    PrintOptional(error_message, expected) << ", Actual: ";
    PrintOptional(error_message, actual) << endl;
  }
}

void ExpectLogSize(ostream& error_message, size_t expected, shared_ptr<SpyLoggerDestination> destination) {
  size_t actual = destination->log.size();
  if (actual != expected) {
    error_message << "Unexpected number of events logged. Expected: " << expected << ", Actual: " << actual << endl;
  }
}

string GetError(ostringstream& error_message) {
  string error = error_message.str();
  if (error.size() > 0) {
    return error;
  }
  return no_errors;
}
}  // End namespace

TestResults test_Destination_TypeRangeGettersAndSetters() {
  TestResults results;
  auto set_min_type = [](const Logger::MessageType& type) {
    auto spy = make_shared<SpyLoggerDestination>();
    if (type == Logger::MessageType::Unknown) {
      spy->SetMinType(Logger::MessageType::Debug);
    } else {
      spy->SetMinType(Logger::MessageType::Unknown);
    }
    spy->SetMinType(type);
    return spy->GetMinType();
  };
  results += execute_suite(make_test_suite(
      "CPPUtils::Logger::SetMinType(const MessageType&)",
      set_min_type,
      {
          make_test(
              "should set min type to Unknown", Logger::MessageType::Unknown, make_tuple(Logger::MessageType::Unknown)),
          make_test("should set min type to Debug", Logger::MessageType::Debug, make_tuple(Logger::MessageType::Debug)),
          make_test(
              "should set min type to Verbose", Logger::MessageType::Verbose, make_tuple(Logger::MessageType::Verbose)),
          make_test("should set min type to Info", Logger::MessageType::Info, make_tuple(Logger::MessageType::Info)),
          make_test(
              "should set min type to Warning", Logger::MessageType::Warning, make_tuple(Logger::MessageType::Warning)),
          make_test("should set min type to Error", Logger::MessageType::Error, make_tuple(Logger::MessageType::Error)),
          make_test("should set min type to Wtf", Logger::MessageType::Wtf, make_tuple(Logger::MessageType::Wtf)),
          make_test("should set min type to Unknown for an invalid MessageType",
                    Logger::MessageType::Unknown,
                    (make_tuple((Logger::MessageType)-1))),
      }));
  auto set_max_type = [](const Logger::MessageType& type) {
    auto spy = make_shared<SpyLoggerDestination>();
    if (type == Logger::MessageType::Unknown) {
      spy->SetMaxType(Logger::MessageType::Debug);
    } else {
      spy->SetMaxType(Logger::MessageType::Unknown);
    }
    spy->SetMaxType(type);
    return spy->GetMaxType();
  };
  results += execute_suite(make_test_suite(
      "CPPUtils::Logger::SetMaxType(const MessageType&)",
      set_max_type,
      {
          make_test(
              "should set max type to Unknown", Logger::MessageType::Unknown, make_tuple(Logger::MessageType::Unknown)),
          make_test("should set max type to Debug", Logger::MessageType::Debug, make_tuple(Logger::MessageType::Debug)),
          make_test(
              "should set max type to Verbose", Logger::MessageType::Verbose, make_tuple(Logger::MessageType::Verbose)),
          make_test("should set max type to Info", Logger::MessageType::Info, make_tuple(Logger::MessageType::Info)),
          make_test(
              "should set max type to Warning", Logger::MessageType::Warning, make_tuple(Logger::MessageType::Warning)),
          make_test("should set max type to Error", Logger::MessageType::Error, make_tuple(Logger::MessageType::Error)),
          make_test("should set max type to Wtf", Logger::MessageType::Wtf, make_tuple(Logger::MessageType::Wtf)),
          make_test("should set max type to Unknown for an invalid MessageType",
                    Logger::MessageType::Unknown,
                    (make_tuple((Logger::MessageType)-1))),
      }));
  return results;
}

TestResults test_Logger_GetShared() {
  auto function_to_test = []() {
    auto first = Logger::GetShared();
    auto second = Logger::GetShared();
    return first == second;
  };
  return execute_suite(make_test_suite("CPPUtils::Logger::GetShared()",
                                       function_to_test,
                                       {
                                           make_test("should get the same instance twice", true, make_tuple()),
                                       }));
}

TestResults test_Logger_GetUnique() {
  auto get_unique_is_different = []() {
    auto first = Logger::GetUnique();
    auto second = Logger::GetUnique();
    return first != second;
  };

  auto get_unique_is_not_get_shared = []() {
    auto shared = Logger::GetShared();
    auto unique = Logger::GetUnique();
    return unique != shared;
  };

  TestResults results;

  results += execute_suite(make_test_suite("CPPUtils::Logger::GetUnique()",
                                           get_unique_is_different,
                                           {
                                               make_test("should get two different instances", true, make_tuple()),
                                           }));

  results += execute_suite(
      make_test_suite("CPPUtils::Logger::GetUnique()",
                      get_unique_is_not_get_shared,
                      {
                          make_test("should get and instance that is not the shared instance", true, make_tuple()),
                      }));

  return results;
}

TestResults test_LogUnimplementedMethod() {
#if __cplusplus >= 202002L
  return TestResults().fail("Not implemented for c++20 yet.");

  ostringstream error_message;
  error_message << "Unimplemented method: \"" << location.function_name() << "\" at " << location.file_name() << " ("
                << location.line() << ":" << location.column() << ")";
  LogDebug(error_message.str());
  // TODO: see what this results in when run in c++20 and test for those values.
#else
  auto function_to_test = [](string method_name) -> string {
    auto spy = make_shared<SpyLoggerDestination>();
    auto logger = Logger::GetUnique();
    logger->AddDestination(spy);
#if __cplusplus >= 202002L
    std::source_location loc;
    logger->LogUnimplementedMethod()
#else
    logger->LogUnimplementedMethodReal(method_name);
#endif
        ostringstream error_message;
    ExpectLogSize(error_message, 1, spy);
    if (spy->log.size() > 0) {
      auto message = spy->log.at(0);
      ExpectMessageType(error_message, Logger::MessageType::Debug, message);
      ExpectMessageText(error_message, "Unimplemented method: int main(int argc, char* argv[])", message);
      ExpectMessageException(error_message, nullopt, message);
    }
    return GetError(error_message);
  };
  return execute_suite(make_test_suite(
      "CPPUtils::Logger::LogUnimplementedMethod",
      function_to_test,
      {
          make_test("should log a debug message", no_errors, make_tuple("int main(int argc, char* argv[])")),
      }));
#endif
}

TestResults test_Logger_LogUnhandledError() {
  auto log_unhandled_error = []() {
    auto logger = Logger::GetUnique();
    auto spy = make_shared<SpyLoggerDestination>();
    logger->AddDestination(spy);
    std::exception ex;
    logger->LogUnhandledError(ex);
    ostringstream error_message;
    ExpectLogSize(error_message, 1, spy);
    if (spy->log.size() > 0) {
      auto message = spy->log.at(0);
      ExpectMessageType(error_message, Logger::MessageType::Debug, message);
      ExpectMessageText(error_message, "Unhandled exception", message);
      ExpectMessageException(error_message, ex.what(), message);
    }
    return GetError(error_message);
  };
  return execute_suite(make_test_suite("CPPUtils::Logger::LogUnhandledError(const std::exception&)",
                                       log_unhandled_error,
                                       {
                                           make_test("should log an error", no_errors, make_tuple()),
                                       }));
}

TestResults test_Logger_LogUnimplementedFeature() {
  auto log_unimplemented_feature = []() {
    auto logger = Logger::GetUnique();
    auto spy = make_shared<SpyLoggerDestination>();
    logger->AddDestination(spy);
    string feature_name = "rolling over";
    logger->LogUnimplementedFeature(feature_name);
    ostringstream error_message;
    ExpectLogSize(error_message, 1, spy);
    if (spy->log.size() > 0) {
      auto message = spy->log.at(0);
      ExpectMessageType(error_message, Logger::MessageType::Debug, message);
      ExpectMessageText(error_message, "Unimplemented feature: " + feature_name, message);
      ExpectMessageException(error_message, nullopt, message);
    }
    return GetError(error_message);
  };
  return execute_suite(make_test_suite("CPPUtils::Logger::LogUnimplementedFeature()",
                                       log_unimplemented_feature,
                                       {
                                           make_test("should log an unimplemented feature", no_errors, make_tuple()),
                                       }));
}

TestResults test_Logger_Log_Level() {
  auto log = [](const Logger::MessageType& type,
                const optional<string>& message_text,
                const optional<std::exception>& ex) -> string {
    ostringstream error_message;
    auto logger = Logger::GetUnique();
    auto spy = make_shared<SpyLoggerDestination>();
    logger->AddDestination(spy);
    switch (type) {
      case Logger::MessageType::Debug:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->LogDebug(message_text.value(), ex.value());
          } else {
            logger->LogDebug(message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->LogDebug(ex.value());
          }
        }
        break;
      case Logger::MessageType::Verbose:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->LogVerbose(message_text.value(), ex.value());
          } else {
            logger->LogVerbose(message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->LogVerbose(ex.value());
          }
        }
        break;
      case Logger::MessageType::Info:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->LogInfo(message_text.value(), ex.value());
          } else {
            logger->LogInfo(message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->LogInfo(ex.value());
          }
        }
        break;
      case Logger::MessageType::Warning:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->LogWarning(message_text.value(), ex.value());
          } else {
            logger->LogWarning(message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->LogWarning(ex.value());
          }
        }
        break;
      case Logger::MessageType::Error:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->LogError(message_text.value(), ex.value());
          } else {
            logger->LogError(message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->LogError(ex.value());
          }
        }
        break;
      case Logger::MessageType::Wtf:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->LogWtf(message_text.value(), ex.value());
          } else {
            logger->LogWtf(message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->LogWtf(ex.value());
          }
        }
        break;
      default:
        break;
    }
    ExpectLogSize(error_message, 1, spy);
    if (spy->log.size() > 0) {
      auto message = spy->log.at(0);
      ExpectMessageType(error_message, type, message);
      ExpectMessageText(error_message, message_text, message);
      ExpectMessageException(error_message, ex.has_value() ? make_optional(ex.value().what()) : nullopt, message);
    }
    return GetError(error_message);
  };
  return execute_suite(
      make_test_suite("CPPUtils::Logger::Log*(...)",
                      log,
                      {
                          make_test("should log what a terrible failure with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Wtf,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log what a terrible failure with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Wtf,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log what a terrible failure with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Wtf,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log an error with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Error,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log an error with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Error,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log an error with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Error,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a warning with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Warning,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log a warning with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Warning,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a warning with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Warning,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log an info with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Info,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log an info with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Info,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log an info with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Info,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a debug with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Debug,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log a debug with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Debug,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a debug with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Debug,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a verbose with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Verbose,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log a verbose with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Verbose,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a verbose with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Verbose,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),

                      }));
}

TestResults test_Logger_Log() {
  auto log = [](const Logger::MessageType& type,
                const optional<string>& message_text,
                const optional<std::exception>& ex) -> string {
    ostringstream error_message;
    auto logger = Logger::GetUnique();
    auto spy = make_shared<SpyLoggerDestination>();
    logger->AddDestination(spy);
    switch (type) {
      case Logger::MessageType::Debug:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Debug, message_text.value(), ex.value());
          } else {
            logger->Log(Logger::MessageType::Debug, message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Debug, ex.value());
          }
        }
        break;
      case Logger::MessageType::Verbose:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Verbose, message_text.value(), ex.value());
          } else {
            logger->Log(Logger::MessageType::Verbose, message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Verbose, ex.value());
          }
        }
        break;
      case Logger::MessageType::Info:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Info, message_text.value(), ex.value());
          } else {
            logger->Log(Logger::MessageType::Info, message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Info, ex.value());
          }
        }
        break;
      case Logger::MessageType::Warning:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Warning, message_text.value(), ex.value());
          } else {
            logger->Log(Logger::MessageType::Warning, message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Warning, ex.value());
          }
        }
        break;
      case Logger::MessageType::Error:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Error, message_text.value(), ex.value());
          } else {
            logger->Log(Logger::MessageType::Error, message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Error, ex.value());
          }
        }
        break;
      case Logger::MessageType::Wtf:
        if (message_text.has_value()) {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Wtf, message_text.value(), ex.value());
          } else {
            logger->Log(Logger::MessageType::Wtf, message_text.value());
          }
        } else {
          if (ex.has_value()) {
            logger->Log(Logger::MessageType::Wtf, ex.value());
          }
        }
        break;
      default:
        break;
    }
    ExpectLogSize(error_message, 1, spy);
    if (spy->log.size() > 0) {
      auto message = spy->log.at(0);
      ExpectMessageType(error_message, type, message);
      ExpectMessageText(error_message, message_text, message);
      ExpectMessageException(error_message, ex.has_value() ? make_optional(ex.value().what()) : nullopt, message);
    }
    return GetError(error_message);
  };
  return execute_suite(
      make_test_suite("CPPUtils::Logger::Log(const MessageType&, ...)",
                      log,
                      {
                          make_test("should log what a terrible failure with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Wtf,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log what a terrible failure with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Wtf,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log what a terrible failure with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Wtf,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log an error with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Error,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log an error with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Error,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log an error with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Error,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a warning with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Warning,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log a warning with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Warning,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a warning with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Warning,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log an info with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Info,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log an info with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Info,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log an info with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Info,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a debug with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Debug,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log a debug with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Debug,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a debug with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Debug,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a verbose with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Verbose,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)nullopt)),
                          make_test("should log a verbose with a message and an exception",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Verbose,
                                               (const optional<string>&)"this should never happen",
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),
                          make_test("should log a verbose with a message",
                                    no_errors,
                                    make_tuple(Logger::MessageType::Verbose,
                                               (const optional<string>&)nullopt,
                                               (const optional<std::exception>&)runtime_error("bad thing happen"))),

                      }));
}

TestResults test_Logger_LogToDo() {
  auto log_to_do = [](const string& todo_message) {
    auto logger = Logger::GetUnique();
    auto spy = make_shared<SpyLoggerDestination>();
    logger->AddDestination(spy);
    logger->LogToDo(todo_message);
    ostringstream error_message;
    ExpectLogSize(error_message, 1, spy);
    if (spy->log.size() > 0) {
      auto message = spy->log.at(0);
      ExpectMessageType(error_message, Logger::MessageType::Debug, message);
      ExpectMessageText(error_message, "TODO: " + todo_message, message);
      ExpectMessageException(error_message, nullopt, message);
    }
    return GetError(error_message);
  };
  return execute_suite(make_test_suite(
      "CPPUtils::Logger::LogToDo(const std:;string&)",
      log_to_do,
      {
          make_test("should log a TODO for \"fill in this function\"", no_errors, make_tuple("fill in this function")),
          make_test("should log a TODO for \"delete this after fixing bug:2048\"",
                    no_errors,
                    make_tuple("delete this after fixing bug:2048")),
          make_test("should log a TODO for \"refactor this\"", no_errors, make_tuple("refactor this")),
      }));
}

TestResults test_Logger_AddDestination_and_ClearDestinations() {
  auto add_destination = []() {
    ostringstream error_message;
    auto logger = Logger::GetUnique();
    auto spy = make_shared<SpyLoggerDestination>();
    ExpectLogSize(error_message, 0, spy);
    logger->LogInfo("first message");
    ExpectLogSize(error_message, 0, spy);
    logger->AddDestination(spy);
    ExpectLogSize(error_message, 0, spy);
    logger->LogInfo("second message");
    ExpectLogSize(error_message, 1, spy);
    logger->ClearDestinations();
    ExpectLogSize(error_message, 1, spy);
    logger->LogInfo("third message");
    ExpectLogSize(error_message, 1, spy);
    if (spy->log.size() > 0) {
      auto message = spy->log.at(0);
      ExpectMessageType(error_message, Logger::MessageType::Info, message);
      ExpectMessageText(error_message, "second message", message);
      ExpectMessageException(error_message, nullopt, message);
    }
    return GetError(error_message);
  };
  return execute_suite(make_test_suite("",
                                       add_destination,
                                       {
                                           make_test("should add and clear destinations", no_errors, make_tuple()),
                                       }));
}

// AddDestination
// ClearDestinations

int main(int argc, char* argv[]) {
  TestResults results;

  results += test_Destination_TypeRangeGettersAndSetters();
  results += test_Logger_GetShared();
  results += test_Logger_GetUnique();
  results += test_LogUnimplementedMethod();
  results += test_Logger_LogUnhandledError();
  results += test_Logger_LogUnimplementedFeature();
  results += test_Logger_Log_Level();
  results += test_Logger_Log();
  results += test_Logger_LogToDo();
  results += test_Logger_AddDestination_and_ClearDestinations();

  PrintResults(cout, results);

  return results.failed() + results.errors();
}
