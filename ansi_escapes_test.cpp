/***************************************************************************************
 * @file ansi_escapes_test.cpp                                                         *
 *                                                                                     *
 * @brief Defines constants and functions for working with screen colors.              *
 * @copyright                                                                          *
 * Copyright 2023 Tom Hicks                                                            *
 * Licensed under the MIT license see the LICENSE file for details.                    *
 ***************************************************************************************/
// clang-format off
#include "ansi_escapes.h"
#include "tinytest.h"

// clang-format on
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>

namespace {
using std::make_tuple;
using std::ostream;
using std::ostringstream;
using std::string;
using std::string_view;
using TinyTest::execute_suite;
using TinyTest::make_test;
using TinyTest::make_test_suite;
using TinyTest::TestResults;
}  // End namespace

string filter(const string& text) {
  std::regex pattern("\033");
  return std::regex_replace(text, pattern, "\\033");
}

TestResults test_GetRedComponent() {
  return execute_suite(make_test_suite(
      "CPPUtils::GetRedComponent(uint32_t)",
      CPPUtils::GetRedComponent,
      {
          make_test("should get the red component 0x34 from 0x12345678", 0x34U, make_tuple(0x12345678U)),
          make_test("should get the red component 0x56 from 0x34567890", 0x56U, make_tuple(0x34567890U)),
      }));
}

TestResults test_GetGreenComponent() {
  return execute_suite(make_test_suite(
      "CPPUtils::GetGreenComponent(uint32_t)",
      CPPUtils::GetGreenComponent,
      {
          make_test("should get the green component 0x56 from 0x12345678", 0x56U, make_tuple(0x12345678U)),
          make_test("should get the green component 0x78 from 0x34567890", 0x78U, make_tuple(0x34567890U)),
      }));
}

TestResults test_GetBlueComponent() {
  return execute_suite(
      make_test_suite("CPPUtils::GetBlueComponent(uint32_t)",
                      CPPUtils::GetBlueComponent,
                      {
                          make_test("should get the blue component 0x78 from 0x12345678", 0x78, make_tuple(0x12345678)),
                          make_test("should get the blue component 0x90 from 0x34567890", 0x90, make_tuple(0x34567890)),
                      }));
}

TestResults test_EscapeWithBasicString() {
  auto function_to_test = [](string text) {
    ostringstream os;
    CPPUtils::Escape(os, text);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::Escape(string)",
      function_to_test,
      {
          make_test("should escape \"asdf\" to \"\\033[asdfm\"", (string) "\033[asdfm", make_tuple("asdf")),
          make_test("should escape \"fdsa\" to \"\\033[fdsam\"", (string) "\033[fdsam", make_tuple("fdsa")),
          make_test("should escape \"1;2;3\" to \"\\033[1;2;3m\"", (string) "\033[1;2;3m", make_tuple("1;2;3")),
      }));
}

TestResults test_EscapeWithBasicStringView() {
  auto function_to_test = [](string_view text) {
    ostringstream os;
    CPPUtils::Escape(os, text);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::Escape(string_view)",
      function_to_test,
      {
          make_test("should escape \"asdf\" to \"\\033[asdfm\"", (string) "\033[asdfm", make_tuple("asdf")),
          make_test("should escape \"fdsa\" to \"\\033[fdsam\"", (string) "\033[fdsam", make_tuple("fdsa")),
          make_test("should escape \"1;2;3\" to \"\\033[1;2;3m\"", (string) "\033[1;2;3m", make_tuple("1;2;3")),
      }));
}

TestResults test_EscapeWithConstCharStar() {
  auto function_to_test = [](const char* text) {
    ostringstream os;
    CPPUtils::Escape(os, text);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::Escape(const char*)",
      function_to_test,
      {
          make_test("should escape \"asdf\" to \"\\033[asdfm\"", (string) "\033[asdfm", make_tuple("asdf")),
          make_test("should escape \"fdsa\" to \"\\033[fdsam\"", (string) "\033[fdsam", make_tuple("fdsa")),
          make_test("should escape \"1;2;3\" to \"\\033[1;2;3m\"", (string) "\033[1;2;3m", make_tuple("1;2;3")),
      }));
}

TestResults test_ForegroundColor8Bit() {
  auto function_to_test = [](uint8_t color) {
    ostringstream os;
    CPPUtils::ForegroundColor8Bit(os, color);
    return os.str();
  };
  return execute_suite(
      make_test_suite("CPPUtils::ForegroundColor8Bit(uint8_t)",
                      function_to_test,
                      {
                          make_test("should write \"\\033[38;5;7m\"", (string) "\033[38;5;7m", make_tuple(0x07U)),
                          make_test("should write \"\\033[38;5;1m\"", (string) "\033[38;5;1m", make_tuple(0x01U)),
                          make_test("should write \"\\033[38;5;11m\"", (string) "\033[38;5;11m", make_tuple(0x0BU)),
                      }));
}

TestResults test_BackgroundColor8Bit() {
  auto function_to_test = [](uint8_t color) {
    ostringstream os;
    CPPUtils::BackgroundColor8Bit(os, color);
    return os.str();
  };
  return execute_suite(
      make_test_suite("CPPUtils::BackgroundColor8Bit(uint8_t)",
                      function_to_test,
                      {
                          make_test("should write \"\\033[48;5;7m\"", (string) "\033[48;5;7m", make_tuple(0x07U)),
                          make_test("should write \"\\033[48;5;1m\"", (string) "\033[48;5;1m", make_tuple(0x01U)),
                          make_test("should write \"\\033[48;5;11m\"", (string) "\033[48;5;11m", make_tuple(0x0BU)),
                      }));
}

TestResults test_ForegroundTrueColorWithUInt32() {
  auto function_to_test = [](uint32_t color) {
    ostringstream os;
    CPPUtils::ForegroundTrueColor(os, color);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::ForegroundTrueColor(uint32_t)",
      function_to_test,
      {
          make_test("should write \"\\033[38;2;21;69;136m\"", (string) "\033[38;2;21;69;136m", make_tuple(0x00154588)),
      }));
}

TestResults test_BackgroundTrueColorWithUInt32() {
  auto function_to_test = [](uint32_t color) {
    ostringstream os;
    CPPUtils::BackgroundTrueColor(os, color);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::BackgroundTrueColor(uint32_t)",
      function_to_test,
      {
          make_test("should write \"\\033[48;2;21;69;136m\"", (string) "\033[48;2;21;69;136m", make_tuple(0x00154588)),
      }));
}

TestResults test_ForegroundTrueColorWith3UInt8() {
  auto function_to_test = [](uint8_t red, uint8_t green, uint8_t blue) {
    ostringstream os;
    CPPUtils::ForegroundTrueColor(os, red, green, blue);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::ForegroundTrueColor(uint8_t, uint8_t, uint8_t)",
      function_to_test,
      {
          make_test(
              "should write \"\\033[38;2;21;69;136m\"", (string) "\033[38;2;21;69;136m", make_tuple(0x15, 0x45, 0x88)),
      }));
}

TestResults test_BackgroundTrueColorWith3UInt8() {
  auto function_to_test = [](uint8_t red, uint8_t green, uint8_t blue) {
    ostringstream os;
    CPPUtils::BackgroundTrueColor(os, red, green, blue);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::BackgroundTrueColor(uint8_t, uint8_t, uint8_t)",
      function_to_test,
      {
          make_test(
              "should write \"\\033[48;2;21;69;136m\"", (string) "\033[48;2;21;69;136m", make_tuple(0x15, 0x45, 0x88)),
      }));
}

TestResults test_Reset() {
  auto function_to_test = []() {
    ostringstream os;
    CPPUtils::Reset(os);
    return os.str();
  };
  return execute_suite(make_test_suite("CPPUtils::Reset",
                                       function_to_test,
                                       {
                                           make_test("should write \"\\033[m\"", (string) "\033[m", make_tuple()),
                                       }));
}

int main(int argc, char* argv[]) {
  TestResults results;

  results += test_GetRedComponent();
  results += test_GetGreenComponent();
  results += test_GetBlueComponent();
  results += test_EscapeWithBasicString();
  results += test_EscapeWithBasicStringView();
  results += test_EscapeWithConstCharStar();
  results += test_ForegroundColor8Bit();
  results += test_BackgroundColor8Bit();
  results += test_ForegroundTrueColorWithUInt32();
  results += test_BackgroundTrueColorWithUInt32();
  results += test_ForegroundTrueColorWith3UInt8();
  results += test_BackgroundTrueColorWith3UInt8();
  results += test_Reset();

  return results.failed() + results.errors();
}
