/**********************************************************************************************************************
 *                                                                                                                    *
 * @file pretty_print_test.cpp                                                                                        *
 *                                                                                                                    *
 * @brief Defines test for the printing functions declared in pretty_print.h.                                         *
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
// clang-format off
#include "pretty_print.h"
#include "tinytest.h"
// clang-format on
#include <deque>
#include <functional>
#include <initializer_list>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

namespace {
using std::make_tuple;
using std::ostringstream;
using std::string;
using std::string_view;
using TinyTest::execute_suite;
using TinyTest::make_test;
using TinyTest::make_test_suite;
using TinyTest::TestResults;
using namespace CPPUtils;
using std::wstring;
} // End namespace

TestResults test_EscapeForPrintingWithAConstCharPointer() {
  TestResults results;
  auto escape_const_char_pointer = [](const char *value) -> string { return EscapeForPrinting(value); };
  results += execute_suite(make_test_suite(
      "CPPUtils::EscapeForPrinting(const char*)",
      escape_const_char_pointer,
      {
          make_test("should escape an empty string", (string) "", make_tuple("")),
          make_test("should esacpe a string with no special characters",
                    (string) "This is a normal string.",
                    make_tuple("This is a normal string.")),
          make_test("should escape the escape character by itself", (string) "\\033", make_tuple("\033")),
          make_test("should escape the escape character within a string",
                    (string) "This string has an \\033 in it.",
                    make_tuple("This string has an \033 in it.")),
      }));
  return results;
}

TestResults test_EscapeForPrintingWithAString() {
  TestResults results;
  auto escape_string = [](const string &value) -> string { return EscapeForPrinting(value); };
  results += execute_suite(make_test_suite(
      "CPPUtils::EscapeForPrinting(const std::string&)",
      escape_string,
      {
          make_test("should escape an empty string", (string) "", make_tuple((string) "")),
          make_test("should escape a string with no special characters",
                    (string) "This is a normal string.",
                    make_tuple((string) "This is a normal string.")),
          make_test("should escape the escape character by itself", (string) "\\033", make_tuple((string) "\033")),
          make_test("should escape the escape character within a string",
                    (string) "This string has an \\033 in it.",
                    make_tuple((string) "This string has an \033 in it.")),
      }));
  return results;
}

TestResults test_EscapeForPrintingWithAStringView() {
  auto escape_string_view = [](const string_view &value) -> string { return EscapeForPrinting(value); };
  return execute_suite(make_test_suite(
      "CPPUtils::EscapeForPrinting(const std::string_view&)",
      escape_string_view,
      {
          make_test("should escape an empty string", (string) "", make_tuple((string_view) "")),
          make_test("should escape a string with no special characters",
                    (string) "This is a normal string.",
                    make_tuple((string_view) "This is a normal string.")),
          make_test("should escape the escape character by itself", (string) "\\033", make_tuple((string_view) "\033")),
          make_test("should escape the escape character within a string",
                    (string) "This string has an \\033 in it.",
                    make_tuple((string_view) "This string has an \033 in it.")),
      }));
}

TestResults test_PrettyPrintWithAConstCharPointer() {
  auto pretty_print = [](const char *value) -> string {
    ostringstream os;
    PrettyPrint(os, value);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::PrettyPrint(std::ostream&, const char*)",
      pretty_print,
      {
          make_test("should print \"\" for an empty string", (string) "\"\"", make_tuple("")),
          make_test("should print \"hello world\"", (string) "\"hello world\"", make_tuple("hello world")),
      }));
}

TestResults test_PrettyPrintWithAString() {
  auto pretty_print = [](const string &value) -> string {
    ostringstream os;
    PrettyPrint(os, value);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::PrettyPrint(std::ostream&, const std::string&)",
      pretty_print,
      {
          make_test("should print \"\" for an empty string", (string) "\"\"", make_tuple((string) "")),
          make_test("should print \"hello world\"", (string) "\"hello world\"", make_tuple((string) "hello world")),
      }));
}

TestResults test_PrettyPrintWithAStringView() {
  auto pretty_print = [](const std::string_view &value) -> string {
    ostringstream os;
    PrettyPrint(os, value);
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::PrettyPrint(std::ostream&, const std::string_view&)",
      pretty_print,
      {
          make_test("should print \"\" for an empty string", (string) "\"\"", make_tuple((string_view) "")),
          make_test(
              "should print \"hello world\"", (string) "\"hello world\"", make_tuple((string_view) "hello world")),
      }));
}

TestResults test_PrettyPrintWithATuple() {
  auto pretty_print = [](int i) -> string {
    ostringstream os;
    switch (i) {
    case 1:
      PrettyPrint(os, make_tuple(1, "hello", 9));
      break;
    case 2:
      PrettyPrint(os, make_tuple());
      break;
    case 3:
      PrettyPrint(os, make_tuple("one", "two", "three"));
      break;
    }
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::PrettyPrint(std::ostream&, std::tuple)",
      pretty_print,
      {
          make_test("should print a tuple of mixed types", (string) "[ 1, \"hello\", 9 ]", make_tuple(1)),
          make_test("should print an empty tuple", (string) "[]", make_tuple(2)),
          make_test("should print a tuple of strings", (string) "[ \"one\", \"two\", \"three\" ]", make_tuple(3)),
      }));
}

TestResults test_PrettyPrintWithAnInitializerList() {
  auto pretty_print = [](int i) {
    ostringstream os;
    std::initializer_list<string> empty_initializer_list = {};
    switch (i) {
    case 1:
      PrettyPrint(os, {"one", "two", "three"});
      break;
    case 2:
      PrettyPrint(os, empty_initializer_list);
      break;
    case 3:
      PrettyPrint(os, {1, 2, 3});
      break;
    case 4:
      PrettyPrint(os, {"one", "two", "three", "four"});
      break;
    case 5:
      PrettyPrint(os, {1.1, 2.2, 3.3});
      break;
    }
    return os.str();
  };

  return execute_suite(make_test_suite(
      "CPPUtils::PrettyPrint(std::ostream&, std::initializer_list)",
      pretty_print,
      {
          make_test(
              "should print an initializer_list of strings", (string) "[ \"one\", \"two\", \"three\" ]", make_tuple(1)),
          make_test("should print an empty initializer_list", (string) "[]", make_tuple(2)),
          make_test("should print an initializer_list of integers", (string) "[ 1, 2, 3 ]", make_tuple(3)),
          make_test("should print an initializer_list of strings with four elements",
                    (string) "[ \"one\", \"two\", \"three\", \"four\" ]",
                    make_tuple(4)),
          make_test("should print an initializer_list of doubles", (string) "[ 1.1, 2.2, 3.3 ]", make_tuple(5)),
      }));
}

TestResults test_PrettyPrintWithDifferentContainerTypes() {
  auto pretty_print = [](int i) {
    ostringstream os;
    switch (i) {
    case 1: {
      std::vector<int> v = {1, 2, 3};
      PrettyPrint(os, v);
      break;
    }
    case 2: {
      std::queue<int> q;
      q.push(1);
      q.push(2);
      q.push(3);
      PrettyPrint(os, q);
      break;
    }
    case 3: {
      std::vector<string> v = {"one", "two", "three"};
      PrettyPrint(os, v);
      break;
    }
    case 4: {
      std::queue<string> q;
      q.push("one");
      q.push("two");
      q.push("three");
      PrettyPrint(os, q);
      break;
    }
    }
    return os.str();
  };

  return execute_suite(make_test_suite(
      "CPPUtils::PrettyPrint(std::ostream&, TContainer)",
      pretty_print,
      {
          make_test("should print a vector of integers", (string) "[ 1, 2, 3 ]", make_tuple(1)),
          make_test("should print a queue of integers", (string) "[ 1, 2, 3 ]", make_tuple(2)),
          make_test("should print a vector of strings", (string) "[ \"one\", \"two\", \"three\" ]", make_tuple(3)),
          make_test("should print a queue of strings", (string) "[ \"one\", \"two\", \"three\" ]", make_tuple(4)),
      }));
}

TestResults test_PrettyPrintWithSimpleTypes() {
  TestResults results;

  auto pretty_print_int = [](int value) -> string {
    ostringstream os;
    PrettyPrint(os, value);
    return os.str();
  };
  results += execute_suite(make_test_suite("CPPUtils::PrettyPrint(std::ostream&, const TItem&)",
                                           pretty_print_int,
                                           {
                                               make_test("should print 42 for an int", (string) "42", make_tuple(42)),
                                           }));

  auto pretty_print_float = [](float value) -> string {
    ostringstream os;
    PrettyPrint(os, value);
    return os.str();
  };
  results +=
      execute_suite(make_test_suite("CPPUtils::PrettyPrint(std::ostream&, const TItem&)",
                                    pretty_print_float,
                                    {
                                        make_test("should print 3.14 for a float", (string) "3.14", make_tuple(3.14f)),
                                    }));

  auto pretty_print_string = [](const string &value) -> string {
    ostringstream os;
    PrettyPrint(os, value);
    return os.str();
  };
  results += execute_suite(make_test_suite("CPPUtils::PrettyPrint(std::ostream&, const TItem&)",
                                           pretty_print_string,
                                           {
                                               make_test("should print \"hello world\" for a string",
                                                         (string) "\"hello world\"",
                                                         make_tuple((string) "hello world")),
                                           }));
  return results;
}

TestResults test_PrettyPrintWithAPair() {
  auto pretty_print = [](int id) -> string {
    ostringstream os;
    switch (id) {
    case 1:
      PrettyPrint(os, std::make_pair(1, 2));
      break;
    case 2:
      PrettyPrint(os, std::make_pair(3.14f, 42));
      break;
    case 3:
      PrettyPrint(os, std::make_pair((string) "hello", (string) "world"));
      break;
    }
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::PrettyPrint(std::ostream&, const std::pair<T1, T2>&)",
      pretty_print,
      {
          make_test("should print (1, 2) for a pair of ints", (string) "(1, 2)", make_tuple(1)),
          make_test("should print (3.14, 42) for a pair of float and int", (string) "(3.14, 42)", make_tuple(2)),
          make_test("should print (\"hello\", \"world\") for a pair of strings",
                    (string) "(\"hello\", \"world\")",
                    make_tuple(3)),
      }));
}

TestResults test_PrettyPrintWithSeparatorWithAConstCharPointer() {
  auto pretty_print = [](const char *separator, int id) -> string {
    ostringstream os;
    switch (id) {
    case 1:
      PrettyPrintWithSeparator(os, separator, 1, 2, 3);
      break;
    case 2:
      PrettyPrintWithSeparator(os, separator, 3.14f, 42, (string) "hello world");
      break;
    case 3:
      PrettyPrintWithSeparator(os, separator, (string) "hello", (string) "world");
      break;
    }
    return os.str();
  };
  return execute_suite(
      make_test_suite("CPPUtils::PrettyPrintWithSeparator(std::ostream&, const TChar*, Args&&...)",
                      pretty_print,
                      {
                          make_test("should print 1, 2, 3 for a list of ints", (string) "1, 2, 3", make_tuple(", ", 1)),
                          make_test("should print 3.14; 42; \"hello world\" for a list of float, int and string",
                                    (string) "3.14; 42; \"hello world\"",
                                    make_tuple("; ", 2)),
                          make_test("should print \"hello\" \"world\" for a list of strings",
                                    (string) "\"hello\" \"world\"",
                                    make_tuple(" ", 3)),
                      }));
}

TestResults test_PrettyPrintWithSeparatorWithAString() {
  auto pretty_print = [](const string &separator, int id) -> string {
    ostringstream os;
    switch (id) {
    case 1:
      PrettyPrintWithSeparator(os, separator, 1, 2, 3);
      break;
    case 2:
      PrettyPrintWithSeparator(os, separator, 3.14f, 42, (string) "hello world");
      break;
    case 3:
      PrettyPrintWithSeparator(os, separator, (string) "hello", (string) "world");
      break;
    }
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::PrettyPrintWithSeparator(std::ostream&, const TChar*, Args&&...)",
      pretty_print,
      {
          make_test("should print 1 | 2 | 3 for a list of ints", (string) "1 | 2 | 3", make_tuple((string) " | ", 1)),
          make_test("should print 3.14 / 42 / \"hello world\" for a list of float, int and string",
                    (string) "3.14 / 42 / \"hello world\"",
                    make_tuple((string) " / ", 2)),
          make_test("should print \"hello\" - \"world\" for a list of strings",
                    (string) "\"hello\" - \"world\"",
                    make_tuple((string) " - ", 3)),
      }));
}

TestResults test_PrettyPrintWithSeparatorWithAStringView() {
  auto pretty_print = [](const string_view &separator, int id) -> string {
    ostringstream os;
    switch (id) {
    case 1:
      PrettyPrintWithSeparator(os, separator, 1, 2, 3);
      break;
    case 2:
      PrettyPrintWithSeparator(os, separator, 3.14f, 42, "hello world");
      break;
    case 3:
      PrettyPrintWithSeparator(os, separator, "hello", "world");
      break;
    }
    return os.str();
  };
  return execute_suite(make_test_suite(
      "CPPUtils::PrettyPrintWithSeparator(std::ostream&, const TChar*, Args&&...)",
      pretty_print,
      {
          make_test(
              "should print 1 | 2 | 3 for a list of ints", (string) "1 | 2 | 3", make_tuple((string_view) " | ", 1)),
          make_test("should print 3.14 / 42 / \"hello world\" for a list of float, int and string",
                    (string) "3.14 / 42 / \"hello world\"",
                    make_tuple((string_view) " / ", 2)),
          make_test("should print \"hello\" - \"world\" for a list of strings",
                    (string) "\"hello\" - \"world\"",
                    make_tuple((string_view) " - ", 3)),
      }));
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  TestResults results;

  results += test_EscapeForPrintingWithAConstCharPointer();
  results += test_EscapeForPrintingWithAString();
  results += test_EscapeForPrintingWithAStringView();

  results += test_PrettyPrintWithAConstCharPointer();
  results += test_PrettyPrintWithAString();
  results += test_PrettyPrintWithAStringView();
  results += test_PrettyPrintWithATuple();
  results += test_PrettyPrintWithAnInitializerList();
  results += test_PrettyPrintWithDifferentContainerTypes();
  results += test_PrettyPrintWithSimpleTypes();
  results += test_PrettyPrintWithAPair();

  results += test_PrettyPrintWithSeparatorWithAConstCharPointer();
  results += test_PrettyPrintWithSeparatorWithAString();
  results += test_PrettyPrintWithSeparatorWithAStringView();

  return results.failed() + results.errors();
}
