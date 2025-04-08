/***************************************************************************************
 * @file tinytest.h                                                                    *
 *                                                                                     *
 * @brief Defines structs and functions for implementing TinyTest.                     *
 * @copyright Copyright 2023 Tom Hicks <headhunter3@gmail.com>                         *
 * Licensed under the MIT license see the LICENSE file for details.                    *
 ***************************************************************************************/

#include "tinytest.h"

#include <optional>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {
using std::function;
using std::get;
using std::make_tuple;
using std::nullopt;
using std::ostringstream;
using std::string;
using std::tuple;
using std::vector;
using testing::Eq;
using testing::Ne;
using TinyTest::Coalesce;
using TinyTest::Compare;
using TinyTest::DefaultTestCompareFunction;
using TinyTest::DefaultTestConfigureFunction;
using TinyTest::ExecuteSuite;
using TinyTest::InterceptCout;
using TinyTest::MakeTest;
using TinyTest::MakeTestSuite;
using TinyTest::MaybeTestCompareFunction;
using TinyTest::MaybeTestConfigureFunction;
using TinyTest::PrintResults;
using TinyTest::TestResults;
using TinyTest::TestSuite;
using TinyTest::TestTuple;

TEST(VectorCompare, ShouldPrintSizeMismatch) {
  ostringstream os;
  vector first = vector({1, 2, 3, 4});
  vector second = vector({1, 2, 3});
  Compare(os, first, second);
  EXPECT_THAT(os.str(), Eq("size mismatch expected: 4, actual: 3"));
}

TEST(VectorCompare, ShouldPrintVectorsDifferAtIndexZero) {
  ostringstream os;
  vector first = vector({1, 2, 3, 4});
  vector second = vector({0, 1, 2, 3});
  Compare(os, first, second);
  EXPECT_THAT(os.str(),
              Eq((string) "vectors differ at index 0, 1 != 0, expected: [ 1, 2, 3, 4 ], actual: [ 0, 1, 2, 3 ]"));
}

TEST(VectorCompare, ShouldPrintVectorsDifferAtEnd) {
  ostringstream os;
  vector first = vector({1, 2, 3, 4});
  vector second = vector({1, 2, 3, 0});
  Compare(os, first, second);
  EXPECT_THAT(os.str(), Eq("vectors differ at index 3, 4 != 0, expected: [ 1, 2, 3, 4 ], actual: [ 1, 2, 3, 0 ]"));
}

TEST(VectorCompare, ShouldPrintNothingWhenVectorsAreEqual) {
  ostringstream os;
  vector first = vector({1, 2, 3, 4});
  vector second = vector({1, 2, 3, 4});
  Compare(os, first, second);
  EXPECT_THAT(os.str(), Eq(""));
}

TEST(TestResults, ShouldConstructTheDefaultInstance) {
  TestResults actual;
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(0));
  EXPECT_THAT(actual.Errors(), Eq(0));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(0));
  EXPECT_THAT(actual.Failed(), Eq(0));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(0));
  EXPECT_THAT(actual.Skipped(), Eq(0));
  EXPECT_THAT(actual.Passed(), Eq(0));
  EXPECT_THAT(actual.Total(), Eq(0));
}

TEST(TestResults, ShouldCopyAnotherInstance) {
  TestResults original;
  original.Pass().Skip().Skip().Fail().Fail().Fail().Error().Error().Error().Error();
  TestResults actual(original);
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(0));
  EXPECT_THAT(actual.Errors(), Eq(4));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(0));
  EXPECT_THAT(actual.Failed(), Eq(3));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(0));
  EXPECT_THAT(actual.Skipped(), Eq(2));
  EXPECT_THAT(actual.Passed(), Eq(1));
  EXPECT_THAT(actual.Total(), Eq(6));
}

TEST(TestResults, ShouldCreateASpecificInstance) {
  TestResults actual(1, 2, 3, 4, 5, {"hello"}, {"first", "second"}, {"third", "fourth", "fifth"});
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(1));
  EXPECT_THAT(actual.Errors(), Eq(1));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(2));
  EXPECT_THAT(actual.Failed(), Eq(2));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(3));
  EXPECT_THAT(actual.Skipped(), Eq(4));
  EXPECT_THAT(actual.Passed(), Eq(3));
  EXPECT_THAT(actual.Total(), Eq(5));
}

TEST(TestResults, ShouldReportAnErrorWithoutAMessage) {
  TestResults actual;
  actual.Error();
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(0));
  EXPECT_THAT(actual.Errors(), Eq(1));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(0));
  EXPECT_THAT(actual.Failed(), Eq(0));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(0));
  EXPECT_THAT(actual.Skipped(), Eq(0));
  EXPECT_THAT(actual.Passed(), Eq(0));
  EXPECT_THAT(actual.Total(), Eq(0));
}

TEST(TestResults, ShouldReportAnErrorWithAMessage) {
  TestResults actual;
  actual.Error("my error message");
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(1));
  EXPECT_THAT(actual.ErrorMessages()[0], Eq("my error message"));
  EXPECT_THAT(actual.Errors(), Eq(1));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(0));
  EXPECT_THAT(actual.Failed(), Eq(0));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(0));
  EXPECT_THAT(actual.Skipped(), Eq(0));
  EXPECT_THAT(actual.Passed(), Eq(0));
  EXPECT_THAT(actual.Total(), Eq(0));
}

TEST(TestResults, ShouldReportAFailureWithoutAMessage) {
  TestResults actual;
  actual.Fail();
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(0));
  EXPECT_THAT(actual.Errors(), Eq(0));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(0));
  EXPECT_THAT(actual.Failed(), Eq(1));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(0));
  EXPECT_THAT(actual.Skipped(), Eq(0));
  EXPECT_THAT(actual.Passed(), Eq(0));
  EXPECT_THAT(actual.Total(), Eq(1));
}

TEST(TestResults, ShouldReportAFailureWithAMessage) {
  TestResults actual;
  actual.Fail("this test failed");
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(0));
  EXPECT_THAT(actual.Errors(), Eq(0));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(1));
  EXPECT_THAT(actual.FailureMessages()[0], Eq("this test failed"));
  EXPECT_THAT(actual.Failed(), Eq(1));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(0));
  EXPECT_THAT(actual.Skipped(), Eq(0));
  EXPECT_THAT(actual.Passed(), Eq(0));
  EXPECT_THAT(actual.Total(), Eq(1));
}

// You can't currently pass with a message.
TEST(TestResults, ShouldReportAPassWithoutAMessage) {
  TestResults actual;
  actual.Pass();
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(0));
  EXPECT_THAT(actual.Errors(), Eq(0));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(0));
  EXPECT_THAT(actual.Failed(), Eq(0));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(0));
  EXPECT_THAT(actual.Skipped(), Eq(0));
  EXPECT_THAT(actual.Passed(), Eq(1));
  EXPECT_THAT(actual.Total(), Eq(1));
}

TEST(TestResults, ShouldReportASkipWithoutAMessage) {
  TestResults actual;
  actual.Skip();
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(0));
  EXPECT_THAT(actual.Errors(), Eq(0));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(0));
  EXPECT_THAT(actual.Failed(), Eq(0));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(0));
  EXPECT_THAT(actual.Skipped(), Eq(1));
  EXPECT_THAT(actual.Passed(), Eq(0));
  EXPECT_THAT(actual.Total(), Eq(1));
}

TEST(TestResults, ShouldReportASkipWithAMessage) {
  TestResults actual;
  actual.Skip("not ready yet");
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(0));
  EXPECT_THAT(actual.Errors(), Eq(0));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(0));
  EXPECT_THAT(actual.Failed(), Eq(0));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(1));
  EXPECT_THAT(actual.SkipMessages()[0], Eq("not ready yet"));
  EXPECT_THAT(actual.Skipped(), Eq(1));
  EXPECT_THAT(actual.Passed(), Eq(0));
  EXPECT_THAT(actual.Total(), Eq(1));
}

TEST(TestResults, ShouldGetACombinationOfTwoInstances) {
  const TestResults first = TestResults().Pass().Skip("S").Fail("F");
  const TestResults second = TestResults().Skip().Error("A").Error("B").Fail("D");
  const TestResults third = first + second;

  // Expect first was unchanged.
  EXPECT_THAT(first.ErrorMessages().size(), Eq(0));
  EXPECT_THAT(first.Errors(), Eq(0));
  EXPECT_THAT(first.FailureMessages().size(), Eq(1));
  EXPECT_THAT(first.FailureMessages()[0], Eq("F"));
  EXPECT_THAT(first.Failed(), Eq(1));
  EXPECT_THAT(first.SkipMessages().size(), Eq(1));
  EXPECT_THAT(first.SkipMessages()[0], Eq("S"));
  EXPECT_THAT(first.Skipped(), Eq(1));
  EXPECT_THAT(first.Passed(), Eq(1));
  EXPECT_THAT(first.Total(), Eq(3));

  // Expect second was unchanged
  EXPECT_THAT(second.ErrorMessages().size(), Eq(2));
  EXPECT_THAT(second.ErrorMessages()[0], Eq("A"));
  EXPECT_THAT(second.ErrorMessages()[1], Eq("B"));
  EXPECT_THAT(second.Errors(), Eq(2));
  EXPECT_THAT(second.FailureMessages().size(), Eq(1));
  EXPECT_THAT(second.FailureMessages()[0], Eq("D"));
  EXPECT_THAT(second.Failed(), Eq(1));
  EXPECT_THAT(second.SkipMessages().size(), Eq(0));
  EXPECT_THAT(second.Skipped(), Eq(1));
  EXPECT_THAT(second.Passed(), Eq(0));
  EXPECT_THAT(second.Total(), Eq(2));

  // Expect third was the combination of the two.
  EXPECT_THAT(third.ErrorMessages().size(), Eq(2));
  EXPECT_THAT(third.ErrorMessages()[0], Eq("A"));
  EXPECT_THAT(third.ErrorMessages()[1], Eq("B"));
  EXPECT_THAT(third.Errors(), Eq(2));
  EXPECT_THAT(third.FailureMessages().size(), Eq(2));
  EXPECT_THAT(third.FailureMessages()[0], Eq("F"));
  EXPECT_THAT(third.FailureMessages()[1], Eq("D"));
  EXPECT_THAT(third.Failed(), Eq(2));
  EXPECT_THAT(third.SkipMessages().size(), Eq(1));
  EXPECT_THAT(third.SkipMessages()[0], Eq("S"));
  EXPECT_THAT(third.Skipped(), Eq(2));
  EXPECT_THAT(third.Passed(), Eq(1));
  EXPECT_THAT(third.Total(), Eq(5));
}

TEST(TestResults, ShouldCombineAnotherInstanceIntoThisOne) {
  TestResults first = TestResults().Pass().Skip("S").Fail("F");
  const TestResults second = TestResults().Skip().Error("A").Error("B").Fail("D");
  first += second;

  // Expect second was unchanged
  EXPECT_THAT(second.ErrorMessages().size(), Eq(2));
  EXPECT_THAT(second.ErrorMessages()[0], Eq("A"));
  EXPECT_THAT(second.ErrorMessages()[1], Eq("B"));
  EXPECT_THAT(second.Errors(), Eq(2));
  EXPECT_THAT(second.FailureMessages().size(), Eq(1));
  EXPECT_THAT(second.FailureMessages()[0], Eq("D"));
  EXPECT_THAT(second.Failed(), Eq(1));
  EXPECT_THAT(second.SkipMessages().size(), Eq(0));
  EXPECT_THAT(second.Skipped(), Eq(1));
  EXPECT_THAT(second.Passed(), Eq(0));
  EXPECT_THAT(second.Total(), Eq(2));

  // Expect first is the combination of the two.
  EXPECT_THAT(first.ErrorMessages().size(), Eq(2));
  EXPECT_THAT(first.ErrorMessages()[0], Eq("A"));
  EXPECT_THAT(first.ErrorMessages()[1], Eq("B"));
  EXPECT_THAT(first.Errors(), Eq(2));
  EXPECT_THAT(first.FailureMessages().size(), Eq(2));
  EXPECT_THAT(first.FailureMessages()[0], Eq("F"));
  EXPECT_THAT(first.FailureMessages()[1], Eq("D"));
  EXPECT_THAT(first.Failed(), Eq(2));
  EXPECT_THAT(first.SkipMessages().size(), Eq(1));
  EXPECT_THAT(first.SkipMessages()[0], Eq("S"));
  EXPECT_THAT(first.Skipped(), Eq(2));
  EXPECT_THAT(first.Passed(), Eq(1));
  EXPECT_THAT(first.Total(), Eq(5));
}

TEST(TestResults, ShouldCombineAnInstanceWithItself) {
  TestResults actual = TestResults().Pass().Fail("A").Fail("B").Skip().Error("Bad").Skip();
  actual += actual;
  EXPECT_THAT(actual.ErrorMessages().size(), Eq(2));
  EXPECT_THAT(actual.ErrorMessages().at(0), Eq("Bad"));
  EXPECT_THAT(actual.ErrorMessages().at(1), Eq("Bad"));
  EXPECT_THAT(actual.Errors(), Eq(2));
  EXPECT_THAT(actual.FailureMessages().size(), Eq(4));
  EXPECT_THAT(actual.FailureMessages().at(0), Eq("A"));
  EXPECT_THAT(actual.FailureMessages().at(1), Eq("B"));
  EXPECT_THAT(actual.FailureMessages().at(2), Eq("A"));
  EXPECT_THAT(actual.FailureMessages().at(3), Eq("B"));
  EXPECT_THAT(actual.Failed(), Eq(4));
  EXPECT_THAT(actual.SkipMessages().size(), Eq(0));
  EXPECT_THAT(actual.Skipped(), Eq(4));
  EXPECT_THAT(actual.Passed(), Eq(2));
  EXPECT_THAT(actual.Total(), Eq(10));
}

TEST(DefaultTestCompareFunction, ShouldBeNullOpt) {
  auto actual = DefaultTestCompareFunction<int>();
  EXPECT_THAT(actual, Eq(nullopt));
}

TEST(DefaultTestConfigureFunction, ShouldBeNullOpt) {
  auto actual = DefaultTestConfigureFunction();
  EXPECT_THAT(actual, Eq(nullopt));
}

// Test that TestTuple makes the right kind of tuple.
TEST(TestTuple, ShouldDoTheThing) {
  TestTuple<int, string, int, const char*> test = {
      (string) "ASDF",
      0,
      {(string) "first", 2, "third"},
      (MaybeTestCompareFunction<int>)nullopt,
      (MaybeTestConfigureFunction)nullopt,
      (MaybeTestConfigureFunction)nullopt,
      true,
  };
  tuple arguments = get<2>(test);
  EXPECT_THAT(get<0>(test), Eq("ASDF"));
  EXPECT_THAT(get<1>(test), Eq(0));
  EXPECT_THAT(get<2>(test), Eq(make_tuple((string) "first", 2, "third")));
  EXPECT_THAT(get<3>(test), Eq(nullopt));
  EXPECT_THAT(get<4>(test), Eq(nullopt));
  EXPECT_THAT(get<5>(test), Eq(nullopt));
  EXPECT_THAT(get<6>(test), Eq(true));
}

TEST(TestTuple, ShouldCoerceValuesToTheCorrectTypes) {
  TestTuple<int, string, int, const char*> test = {
      "A",
      0,
      {"B", 1, "C"},
      nullopt,
      nullopt,
      nullopt,
      true,
  };
  tuple arguments = get<2>(test);
  EXPECT_THAT(get<0>(test), Eq((string) "A"));
  EXPECT_THAT(get<1>(test), Eq(0));
  EXPECT_THAT(get<2>(test), Eq(make_tuple((string) "B", 1, "C")));
  EXPECT_THAT(get<3>(test), Eq(nullopt));
  EXPECT_THAT(get<4>(test), Eq(nullopt));
  EXPECT_THAT(get<5>(test), Eq(nullopt));
  EXPECT_THAT(get<6>(test), Eq(true));
}

TEST(MakeTest, ShouldMakeTests) {
  MaybeTestCompareFunction<string> test_Compare = [](const string& left, const string& right) -> bool { return false; };
  MaybeTestConfigureFunction after_each = []() {};
  MaybeTestConfigureFunction before_each = []() {};

  tuple first = MakeTest(
      (string) "A Test", (string) "A", make_tuple((string) "ABCDEFG", 0), test_Compare, before_each, after_each, false);

  TestTuple<string, string, int> second =
      MakeTest<string, string, int>("Another Test", "B", make_tuple((string) "ABCDEF", 1));
  TestTuple<string, string, int> third = first;

  EXPECT_THAT(get<0>(first), Eq("A Test"));
  EXPECT_THAT(get<0>(second), Eq("Another Test"));
  EXPECT_THAT(get<0>(third), Eq("A Test"));

  EXPECT_THAT(get<1>(first), Eq("A"));
  EXPECT_THAT(get<1>(second), Eq("B"));
  EXPECT_THAT(get<1>(third), Eq("A"));

  EXPECT_THAT(get<2>(first), Eq(make_tuple((string) "ABCDEFG", 0)));
  EXPECT_THAT(get<2>(second), Eq(make_tuple((string) "ABCDEF", 1)));
  EXPECT_THAT(get<2>(third), Eq(make_tuple((string) "ABCDEFG", 0)));

  // TODO: We can only test Eq(nullopt) or not.
  EXPECT_THAT(get<3>(first), Ne(nullopt));
  EXPECT_THAT(get<3>(second), Eq(nullopt));
  EXPECT_THAT(get<3>(third), Ne(nullopt));

  EXPECT_THAT(get<4>(first), Ne(nullopt));
  EXPECT_THAT(get<4>(second), Eq(nullopt));
  EXPECT_THAT(get<4>(third), Ne(nullopt));

  EXPECT_THAT(get<5>(first), Ne(nullopt));
  EXPECT_THAT(get<5>(second), Eq(nullopt));
  EXPECT_THAT(get<5>(third), Ne(nullopt));

  EXPECT_THAT(get<6>(first), Eq(false));
  EXPECT_THAT(get<6>(second), Eq(true));
  EXPECT_THAT(get<6>(third), Eq(false));

  // TODO: Create make_inputs<string, int> and TestInputParams to reduce the amount of type casting in these lines.
}

TEST(TestSuite, ShouldCoerceValuesToTheCorrectTypes) {
  auto fnToTest = [](const string& text, int position) -> string {
    if (position >= 0 && position < text.size()) {
      return &text.at(position);
    }
    return "";
  };
  MaybeTestCompareFunction<string> test_Compare = [](const string& left, const string& right) -> bool { return false; };
  MaybeTestCompareFunction<string> suite_Compare = [](const string& left, const string& right) -> bool { return true; };
  MaybeTestConfigureFunction after_all = []() {};
  MaybeTestConfigureFunction after_each = []() {};
  MaybeTestConfigureFunction before_all = []() {};
  MaybeTestConfigureFunction before_each = []() {};
  TestTuple<string, string, int> test_run = MakeTest<string, string, int>(
      "Test Name", "Expected", make_tuple((string) "text", 0), test_Compare, before_each, after_each, false);
  TestSuite<string, string, int> first = {
      "Suite Name",
      fnToTest,
      {
          test_run,
      },
      suite_Compare,
      before_all,
      after_all,
      true,
  };
  EXPECT_THAT(get<0>(first), Eq("Suite Name"));
  // EXPECT_THAT(get<1>(first), Eq(fnToTest));
  EXPECT_THAT(get<2>(first).size(), Eq(1));
  EXPECT_THAT(get<3>(first), Ne(nullopt));
  EXPECT_THAT(get<4>(first), Ne(nullopt));
  EXPECT_THAT(get<5>(first), Ne(nullopt));
  EXPECT_THAT(get<6>(first), Eq(true));

  auto test_data = *get<2>(first).begin();
  EXPECT_THAT(get<0>(test_data), Eq("Test Name"));
  EXPECT_THAT(get<1>(test_data), Eq("Expected"));
  // Item 2 is checked below as inputs.
  EXPECT_THAT(get<3>(test_data), Ne(nullopt));
  EXPECT_THAT(get<4>(test_data), Ne(nullopt));
  EXPECT_THAT(get<5>(test_data), Ne(nullopt));
  EXPECT_THAT(get<6>(test_data), Eq(false));

  auto inputs = get<2>(test_data);
  EXPECT_THAT(get<0>(inputs), Eq("text"));
  EXPECT_THAT(get<1>(inputs), Eq(0));
}

TEST(MakeTestSuite, ShouldMakeATestSuiteWithAVectorOfTestRuns) {
  auto fnToTest = [](const string& text, int position) -> string {
    if (position >= 0 && position < text.size()) {
      return &text.at(position);
    }
    return "";
  };
  MaybeTestCompareFunction<string> test_Compare = [](const string& left, const string& right) -> bool { return false; };
  MaybeTestCompareFunction<string> suite_Compare = [](const string& left, const string& right) -> bool { return true; };
  MaybeTestConfigureFunction after_all = []() {};
  MaybeTestConfigureFunction after_each = []() {};
  MaybeTestConfigureFunction before_all = []() {};
  MaybeTestConfigureFunction before_each = []() {};
  TestTuple<string, string, int> test_run = MakeTest<string, string, int>(
      "Test Name", "Expected", make_tuple((string) "text", 0), test_Compare, before_each, after_each, false);
  TestSuite<string, string, int> first =
      MakeTestSuite("Suite Name", fnToTest, {test_run}, suite_Compare, before_all, after_all, false);

  EXPECT_THAT(get<0>(first), Eq("Suite Name"));
  // EXPECT_THAT(get<1>(first), Eq(fnToTest));
  EXPECT_THAT(get<2>(first).size(), Eq(1));
  EXPECT_THAT(get<3>(first), Ne(nullopt));
  EXPECT_THAT(get<4>(first), Ne(nullopt));
  EXPECT_THAT(get<5>(first), Ne(nullopt));
  EXPECT_THAT(get<6>(first), Eq(false));

  auto test_data = *get<2>(first).begin();
  EXPECT_THAT(get<0>(test_data), Eq("Test Name"));
  EXPECT_THAT(get<1>(test_data), Eq("Expected"));
  // Item 2 is checked below as inputs.
  EXPECT_THAT(get<3>(test_data), Ne(nullopt));
  EXPECT_THAT(get<4>(test_data), Ne(nullopt));
  EXPECT_THAT(get<5>(test_data), Ne(nullopt));
  EXPECT_THAT(get<6>(test_data), Eq(false));

  auto inputs = get<2>(test_data);
  EXPECT_THAT(get<0>(inputs), Eq("text"));
  EXPECT_THAT(get<1>(inputs), Eq(0));
}

TEST(MakeTestSuite, ShouldMakeATestSuiteWithAnInitializerListOfTestRuns) {
  auto fnToTest = [](const string& text, int position) -> string {
    if (position >= 0 && position < text.size()) {
      return &text.at(position);
    }
    return "";
  };
  MaybeTestCompareFunction<string> test_Compare = [](const string& left, const string& right) -> bool { return false; };
  MaybeTestCompareFunction<string> suite_Compare = [](const string& left, const string& right) -> bool { return true; };
  MaybeTestConfigureFunction after_all = []() {};
  MaybeTestConfigureFunction after_each = []() {};
  MaybeTestConfigureFunction before_all = []() {};
  MaybeTestConfigureFunction before_each = []() {};
  TestTuple<string, string, int> test_run = MakeTest<string, string, int>(
      "Test Name", "Expected", make_tuple((string) "text", 0), test_Compare, before_each, after_each, false);
  TestSuite<string, string, int> first =
      MakeTestSuite("Suite Two", fnToTest, {test_run}, suite_Compare, before_all, after_all, true);

  EXPECT_THAT(get<0>(first), Eq("Suite Two"));
  // EXPECT_THAT(get<1>(first), Eq(fnToTest));
  EXPECT_THAT(get<2>(first).size(), Eq(1));
  EXPECT_THAT(get<3>(first), Ne(nullopt));
  EXPECT_THAT(get<4>(first), Ne(nullopt));
  EXPECT_THAT(get<5>(first), Ne(nullopt));
  EXPECT_THAT(get<6>(first), Eq(true));

  auto test_data = *get<2>(first).begin();
  EXPECT_THAT(get<0>(test_data), Eq("Test Name"));
  EXPECT_THAT(get<1>(test_data), Eq("Expected"));
  // Item 2 is checked below as inputs.
  EXPECT_THAT(get<3>(test_data), Ne(nullopt));
  EXPECT_THAT(get<4>(test_data), Ne(nullopt));
  EXPECT_THAT(get<5>(test_data), Ne(nullopt));
  EXPECT_THAT(get<6>(test_data), Eq(false));

  auto inputs = get<2>(test_data);
  EXPECT_THAT(get<0>(inputs), Eq("text"));
  EXPECT_THAT(get<1>(inputs), Eq(0));
}

TEST(PrintResults, ShouldDoTheThing) {
  TestResults results;
  results.Error()
      .Fail()
      .Fail()
      .Skip()
      .Skip()
      .Skip()
      .Pass()
      .Pass()
      .Pass()
      .Pass()
      .Pass()
      .Skip("skip with a message")
      .Fail("fail with a message")
      .Error("error with a message");
  ostringstream os;
  PrintResults(os, results);
  EXPECT_THAT(os.str(), Eq(R"test(Skipped:
🚧Skipped: skip with a message
Failures:
❌FAILED: fail with a message
Errors:
🔥ERROR: error with a message
Total tests: 12
Passed:      5 ✅
Failed:      3 ❌
Skipped:     4 🚧
Errors:      2 🔥
)test"));
}

TEST(Coalesce, ShouldCombineTwoNulls) {
  MaybeTestConfigureFunction fn1 = nullopt;
  MaybeTestConfigureFunction fn2 = nullopt;
  MaybeTestConfigureFunction actual = Coalesce(fn1, fn2);
  EXPECT_THAT(actual, Eq(nullopt));
  EXPECT_THAT(actual.has_value(), Eq(false));
}

TEST(Coalesce, ShouldCombineAFunctionWithANull) {
  bool this_starts_false;
  MaybeTestConfigureFunction fn1 = [&this_starts_false]() { this_starts_false = true; };
  MaybeTestConfigureFunction fn2 = nullopt;
  MaybeTestConfigureFunction actual = Coalesce(fn1, fn2);
  EXPECT_THAT(actual, Ne(nullopt));
  EXPECT_THAT(actual.has_value(), Eq(true));
  actual.value()();
  EXPECT_THAT(this_starts_false, true);
}

TEST(Coalesce, ShouldCombineANullWithAFunction) {
  bool this_starts_false;
  MaybeTestConfigureFunction fn1 = nullopt;
  MaybeTestConfigureFunction fn2 = [&this_starts_false]() { this_starts_false = true; };
  MaybeTestConfigureFunction actual = Coalesce(fn1, fn2);
  EXPECT_THAT(actual, Ne(nullopt));
  EXPECT_THAT(actual.has_value(), Eq(true));
  actual.value()();
  EXPECT_THAT(this_starts_false, true);
}

TEST(Coalesce, ShouldCombineTwoFunctions) {
  bool red_flag = false;
  bool blue_flag = false;
  MaybeTestConfigureFunction fn1 = [&red_flag]() { red_flag = true; };
  MaybeTestConfigureFunction fn2 = [&blue_flag]() { blue_flag = true; };
  MaybeTestConfigureFunction actual = Coalesce(fn1, fn2);
  EXPECT_THAT(actual, Ne(nullopt));
  EXPECT_THAT(actual.has_value(), Eq(true));
  actual.value()();
  EXPECT_THAT(red_flag, Eq(true));
  EXPECT_THAT(blue_flag, Eq(true));
}

TEST(Coalesce, ShouldExecuteInTheCorrectOrder) {
  vector<string> lines;
  MaybeTestConfigureFunction fn1 = [&lines]() { lines.push_back("Line 1"); };
  MaybeTestConfigureFunction fn2 = [&lines]() { lines.push_back("Line 2"); };
  MaybeTestConfigureFunction actual = Coalesce(fn1, fn2);
  EXPECT_THAT(actual, Ne(nullopt));
  EXPECT_THAT(actual.has_value(), Eq(true));
  actual.value()();
  EXPECT_THAT(lines.size(), Eq(2));
  EXPECT_THAT(lines.at(0), Eq("Line 1"));
  EXPECT_THAT(lines.at(1), Eq("Line 2"));
}

TEST(ExecuteSuiteWithParams, ShouldNotExecuteADisabledSuite) {
  bool suite_Compare_called = false;
  MaybeTestCompareFunction<bool> suite_Compare = [&suite_Compare_called](bool left, bool right) {
    suite_Compare_called = true;
    return left == right;
  };
  bool before_all_called = false;
  MaybeTestConfigureFunction before_all = [&before_all_called]() { before_all_called = true; };
  bool after_all_called = false;
  MaybeTestConfigureFunction after_all = [&after_all_called]() { after_all_called = true; };
  bool test_Compare_called = false;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_called](bool left, bool right) {
    test_Compare_called = true;
    return left == right;
  };
  bool before_each_called = false;
  MaybeTestConfigureFunction before_each = [&before_each_called]() { before_each_called = true; };
  bool after_each_called = false;
  MaybeTestConfigureFunction after_each = [&after_each_called]() { after_each_called = true; };
  bool test_function_called = false;
  function<bool()> test_function = [&test_function_called]() {
    test_function_called = true;
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite",
                 test_function,
                 {
                     MakeTest("Test Name", true, make_tuple(), test_Compare, before_each, after_each, true),
                 },
                 suite_Compare,
                 before_all,
                 after_all,
                 false);
  };

  string output = InterceptCout(wrapper);
  EXPECT_THAT(output, Eq(R"test(🚧Skipping suite: My Suite because it is disabled.
  🚧Skipping Test: Test Name because the suite is disabled.
)test"));
  EXPECT_THAT(test_function_called, Eq(false));
  EXPECT_THAT(suite_Compare_called, Eq(false));
  EXPECT_THAT(before_all_called, Eq(false));
  EXPECT_THAT(after_all_called, Eq(false));
  EXPECT_THAT(test_Compare_called, Eq(false));
  EXPECT_THAT(before_each_called, Eq(false));
  EXPECT_THAT(after_each_called, Eq(false));
}

TEST(ExecuteSuiteWithParams, ShouldNotExecuteASuiteWithNoTests) {
  bool suite_Compare_called = false;
  MaybeTestCompareFunction<bool> suite_Compare = [&suite_Compare_called](bool left, bool right) {
    suite_Compare_called = true;
    return left == right;
  };
  bool before_all_called = false;
  MaybeTestConfigureFunction before_all = [&before_all_called]() { before_all_called = true; };
  bool after_all_called = false;
  MaybeTestConfigureFunction after_all = [&after_all_called]() { after_all_called = true; };
  bool test_Compare_called = false;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_called](bool left, bool right) {
    test_Compare_called = true;
    return left == right;
  };
  bool before_each_called = false;
  MaybeTestConfigureFunction before_each = [&before_each_called]() { before_each_called = true; };
  bool after_each_called = false;
  MaybeTestConfigureFunction after_each = [&after_each_called]() { after_each_called = true; };
  bool test_function_called = false;
  function<bool()> test_function = [&test_function_called]() {
    test_function_called = true;
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite", test_function, {}, suite_Compare, before_all, after_all, true);
  };

  string output = InterceptCout(wrapper);
  EXPECT_THAT(output, Eq("🚧Skipping suite: My Suite because it is empty.\n"));
  EXPECT_THAT(test_function_called, Eq(false));
  EXPECT_THAT(suite_Compare_called, Eq(false));
  EXPECT_THAT(before_all_called, Eq(false));
  EXPECT_THAT(after_all_called, Eq(false));
  EXPECT_THAT(test_Compare_called, Eq(false));
  EXPECT_THAT(before_each_called, Eq(false));
  EXPECT_THAT(after_each_called, Eq(false));
}

TEST(ExecuteSuiteWithParams, ShouldExecuteASuiteWithASinglePass) {
  int suite_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> suite_Compare = [&](bool left, bool right) {
    suite_Compare_call_count++;
    return left == right;
  };
  int before_all_call_count = 0;
  MaybeTestConfigureFunction before_all = [&]() { before_all_call_count++; };
  int after_all_call_count = 0;
  MaybeTestConfigureFunction after_all = [&]() { after_all_call_count++; };
  int test_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_call_count](bool left, bool right) {
    test_Compare_call_count++;
    return left == right;
  };
  int before_each_call_count = 0;
  MaybeTestConfigureFunction before_each = [&before_each_call_count]() { before_each_call_count++; };
  int after_each_call_count = 0;
  MaybeTestConfigureFunction after_each = [&after_each_call_count]() { after_each_call_count++; };
  int test_function_call_count = 0;
  function<bool()> test_function = [&test_function_call_count]() {
    test_function_call_count++;
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite",
                 test_function,
                 {
                     MakeTest("Test Name", true, make_tuple(), test_Compare, before_each, after_each, true),
                 },
                 suite_Compare,
                 before_all,
                 after_all,
                 true);
  };

  string output = InterceptCout(wrapper);
  EXPECT_THAT(output,
              Eq(
                  R"test(🚀Beginning Suite: My Suite
  Beginning Test: Test Name
    ✅PASSED
  Ending Test: Test Name
Ending Suite: My Suite
)test"));
  EXPECT_THAT(test_function_call_count, Eq(1));
  EXPECT_THAT(suite_Compare_call_count, Eq(0));
  EXPECT_THAT(before_all_call_count, Eq(1));
  EXPECT_THAT(after_all_call_count, Eq(1));
  EXPECT_THAT(test_Compare_call_count, Eq(1));
  EXPECT_THAT(before_each_call_count, Eq(1));
  EXPECT_THAT(after_each_call_count, Eq(1));
}

TEST(ExecuteSuiteWithParams, ShouldExecuteASuiteWithASingleFailure) {
  int suite_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> suite_Compare = [&](bool left, bool right) {
    suite_Compare_call_count++;
    return left == right;
  };
  int before_all_call_count = 0;
  MaybeTestConfigureFunction before_all = [&]() { before_all_call_count++; };
  int after_all_call_count = 0;
  MaybeTestConfigureFunction after_all = [&]() { after_all_call_count++; };
  int test_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_call_count](bool left, bool right) {
    test_Compare_call_count++;
    return left == right;
  };
  int before_each_call_count = 0;
  MaybeTestConfigureFunction before_each = [&before_each_call_count]() { before_each_call_count++; };
  int after_each_call_count = 0;
  MaybeTestConfigureFunction after_each = [&after_each_call_count]() { after_each_call_count++; };
  int test_function_call_count = 0;
  function<bool()> test_function = [&test_function_call_count]() {
    test_function_call_count++;
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite",
                 test_function,
                 {
                     MakeTest("Test Name", false, make_tuple(), test_Compare, before_each, after_each, true),
                 },
                 suite_Compare,
                 before_all,
                 after_all,
                 true);
  };

  string output = InterceptCout(wrapper);
  EXPECT_THAT(output,
              Eq(
                  R"test(🚀Beginning Suite: My Suite
  Beginning Test: Test Name
    ❌FAILED: expected: 0, actual: 1
  Ending Test: Test Name
Ending Suite: My Suite
)test"));
  EXPECT_THAT(test_function_call_count, Eq(1));
  EXPECT_THAT(suite_Compare_call_count, Eq(0));
  EXPECT_THAT(before_all_call_count, Eq(1));
  EXPECT_THAT(after_all_call_count, Eq(1));
  EXPECT_THAT(test_Compare_call_count, Eq(1));
  EXPECT_THAT(before_each_call_count, Eq(1));
  EXPECT_THAT(after_each_call_count, Eq(1));
}

TEST(ExecuteSuiteWithParams, ShouldExecuteASuiteWithASingleSkip) {
  int suite_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> suite_Compare = [&](bool left, bool right) {
    suite_Compare_call_count++;
    return left == right;
  };
  int before_all_call_count = 0;
  MaybeTestConfigureFunction before_all = [&]() { before_all_call_count++; };
  int after_all_call_count = 0;
  MaybeTestConfigureFunction after_all = [&]() { after_all_call_count++; };
  int test_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_call_count](bool left, bool right) {
    test_Compare_call_count++;
    return left == right;
  };
  int before_each_call_count = 0;
  MaybeTestConfigureFunction before_each = [&before_each_call_count]() { before_each_call_count++; };
  int after_each_call_count = 0;
  MaybeTestConfigureFunction after_each = [&after_each_call_count]() { after_each_call_count++; };
  int test_function_call_count = 0;
  function<bool()> test_function = [&test_function_call_count]() {
    test_function_call_count++;
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite",
                 test_function,
                 {
                     MakeTest("Test Name", true, make_tuple(), test_Compare, before_each, after_each, false),
                 },
                 suite_Compare,
                 before_all,
                 after_all,
                 true);
  };

  string output = InterceptCout(wrapper);
  EXPECT_THAT(output,
              Eq(
                  R"test(🚀Beginning Suite: My Suite
  🚧Skipping Test: Test Name
Ending Suite: My Suite
)test"));
  EXPECT_THAT(test_function_call_count, Eq(0));
  EXPECT_THAT(suite_Compare_call_count, Eq(0));
  EXPECT_THAT(before_all_call_count, Eq(1));
  EXPECT_THAT(after_all_call_count, Eq(1));
  EXPECT_THAT(test_Compare_call_count, Eq(0));
  EXPECT_THAT(before_each_call_count, Eq(0));
  EXPECT_THAT(after_each_call_count, Eq(0));
}

TEST(ExecuteSuiteWithParams, ShouldExecuteASuiteWithASinglePassAndADisabledTest) {
  int suite_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> suite_Compare = [&](bool left, bool right) {
    suite_Compare_call_count++;
    return left == right;
  };
  int before_all_call_count = 0;
  MaybeTestConfigureFunction before_all = [&]() { before_all_call_count++; };
  int after_all_call_count = 0;
  MaybeTestConfigureFunction after_all = [&]() { after_all_call_count++; };
  int test_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_call_count](bool left, bool right) {
    test_Compare_call_count++;
    return left == right;
  };
  int before_each_call_count = 0;
  MaybeTestConfigureFunction before_each = [&before_each_call_count]() { before_each_call_count++; };
  int after_each_call_count = 0;
  MaybeTestConfigureFunction after_each = [&after_each_call_count]() { after_each_call_count++; };
  int test_function_call_count = 0;
  function<bool()> test_function = [&test_function_call_count]() {
    test_function_call_count++;
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite",
                 test_function,
                 {
                     MakeTest("Test Name", true, make_tuple(), test_Compare, before_each, after_each, true),
                     MakeTest("Second Test", false, make_tuple(), test_Compare, before_each, after_each, false),
                 },
                 suite_Compare,
                 before_all,
                 after_all,
                 true);
  };

  string output = InterceptCout(wrapper);
  EXPECT_THAT(output,
              Eq(
                  R"test(🚀Beginning Suite: My Suite
  Beginning Test: Test Name
    ✅PASSED
  Ending Test: Test Name
  🚧Skipping Test: Second Test
Ending Suite: My Suite
)test"));
  EXPECT_THAT(test_function_call_count, Eq(1));
  EXPECT_THAT(suite_Compare_call_count, Eq(0));
  EXPECT_THAT(before_all_call_count, Eq(1));
  EXPECT_THAT(after_all_call_count, Eq(1));
  EXPECT_THAT(test_Compare_call_count, Eq(1));
  EXPECT_THAT(before_each_call_count, Eq(1));
  EXPECT_THAT(after_each_call_count, Eq(1));
}

TEST(ExecuteSuiteWithParams, ShouldCatchAnExceptionThrownByATest) {
  int suite_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> suite_Compare = [&](bool left, bool right) {
    suite_Compare_call_count++;
    return left == right;
  };
  int before_all_call_count = 0;
  MaybeTestConfigureFunction before_all = [&]() { before_all_call_count++; };
  int after_all_call_count = 0;
  MaybeTestConfigureFunction after_all = [&]() { after_all_call_count++; };
  int test_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_call_count](bool left, bool right) {
    test_Compare_call_count++;
    return left == right;
  };
  int before_each_call_count = 0;
  MaybeTestConfigureFunction before_each = [&before_each_call_count]() { before_each_call_count++; };
  int after_each_call_count = 0;
  MaybeTestConfigureFunction after_each = [&after_each_call_count]() { after_each_call_count++; };
  int test_function_call_count = 0;
  function<bool()> test_function = [&test_function_call_count]() {
    test_function_call_count++;
    throw(std::exception());
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite",
                 test_function,
                 {
                     MakeTest("Test Name", true, make_tuple(), test_Compare, before_each, after_each, true),
                 },
                 suite_Compare,
                 before_all,
                 after_all,
                 true);
  };

  string output = InterceptCout(wrapper);
  // string output = "";
  // EXPECT_THROW((output = InterceptCout(wrapper)), std::exception);
  EXPECT_THAT(output,
              Eq(
                  R"test(🚀Beginning Suite: My Suite
  Beginning Test: Test Name
    🔥ERROR: Caught exception "std::exception".
    ❌FAILED: expected: 1, actual: 0
  Ending Test: Test Name
Ending Suite: My Suite
)test"));
  EXPECT_THAT(test_function_call_count, Eq(1));
  EXPECT_THAT(suite_Compare_call_count, Eq(0));
  EXPECT_THAT(before_all_call_count, Eq(1));
  EXPECT_THAT(after_all_call_count, Eq(1));
  EXPECT_THAT(test_Compare_call_count, Eq(1));
  EXPECT_THAT(before_each_call_count, Eq(1));
  EXPECT_THAT(after_each_call_count, Eq(1));
}

TEST(ExecuteSuiteWithParams, ShouldCatchAStringThrownByATest) {
  int suite_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> suite_Compare = [&](bool left, bool right) {
    suite_Compare_call_count++;
    return left == right;
  };
  int before_all_call_count = 0;
  MaybeTestConfigureFunction before_all = [&]() { before_all_call_count++; };
  int after_all_call_count = 0;
  MaybeTestConfigureFunction after_all = [&]() { after_all_call_count++; };
  int test_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_call_count](bool left, bool right) {
    test_Compare_call_count++;
    return left == right;
  };
  int before_each_call_count = 0;
  MaybeTestConfigureFunction before_each = [&before_each_call_count]() { before_each_call_count++; };
  int after_each_call_count = 0;
  MaybeTestConfigureFunction after_each = [&after_each_call_count]() { after_each_call_count++; };
  int test_function_call_count = 0;
  function<bool()> test_function = [&test_function_call_count]() {
    test_function_call_count++;
    throw((string) "burp");
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite",
                 test_function,
                 {
                     MakeTest("Test Name", true, make_tuple(), test_Compare, before_each, after_each, true),
                 },
                 suite_Compare,
                 before_all,
                 after_all,
                 true);
  };

  string output = InterceptCout(wrapper);
  // string output = "";
  // EXPECT_THROW((output = InterceptCout(wrapper)), std::exception);
  EXPECT_THAT(output,
              Eq(
                  R"test(🚀Beginning Suite: My Suite
  Beginning Test: Test Name
    🔥ERROR: Caught string "burp".
    ❌FAILED: expected: 1, actual: 0
  Ending Test: Test Name
Ending Suite: My Suite
)test"));
  EXPECT_THAT(test_function_call_count, Eq(1));
  EXPECT_THAT(suite_Compare_call_count, Eq(0));
  EXPECT_THAT(before_all_call_count, Eq(1));
  EXPECT_THAT(after_all_call_count, Eq(1));
  EXPECT_THAT(test_Compare_call_count, Eq(1));
  EXPECT_THAT(before_each_call_count, Eq(1));
  EXPECT_THAT(after_each_call_count, Eq(1));
}

TEST(ExecuteSuiteWithParams, ShouldCatchACStringThrownByATest) {
  int suite_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> suite_Compare = [&](bool left, bool right) {
    suite_Compare_call_count++;
    return left == right;
  };
  int before_all_call_count = 0;
  MaybeTestConfigureFunction before_all = [&]() { before_all_call_count++; };
  int after_all_call_count = 0;
  MaybeTestConfigureFunction after_all = [&]() { after_all_call_count++; };
  int test_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_call_count](bool left, bool right) {
    test_Compare_call_count++;
    return left == right;
  };
  int before_each_call_count = 0;
  MaybeTestConfigureFunction before_each = [&before_each_call_count]() { before_each_call_count++; };
  int after_each_call_count = 0;
  MaybeTestConfigureFunction after_each = [&after_each_call_count]() { after_each_call_count++; };
  int test_function_call_count = 0;
  function<bool()> test_function = [&test_function_call_count]() {
    test_function_call_count++;
    throw "burp";
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite",
                 test_function,
                 {
                     MakeTest("Test Name", true, make_tuple(), test_Compare, before_each, after_each, true),
                 },
                 suite_Compare,
                 before_all,
                 after_all,
                 true);
  };

  string output = InterceptCout(wrapper);
  // string output = "";
  // EXPECT_THROW((output = InterceptCout(wrapper)), std::exception);
  EXPECT_THAT(output,
              Eq(
                  R"test(🚀Beginning Suite: My Suite
  Beginning Test: Test Name
    🔥ERROR: Caught c-string "burp".
    ❌FAILED: expected: 1, actual: 0
  Ending Test: Test Name
Ending Suite: My Suite
)test"));
  EXPECT_THAT(test_function_call_count, Eq(1));
  EXPECT_THAT(suite_Compare_call_count, Eq(0));
  EXPECT_THAT(before_all_call_count, Eq(1));
  EXPECT_THAT(after_all_call_count, Eq(1));
  EXPECT_THAT(test_Compare_call_count, Eq(1));
  EXPECT_THAT(before_each_call_count, Eq(1));
  EXPECT_THAT(after_each_call_count, Eq(1));
}

TEST(ExecuteSuiteWithParams, ShouldCatchSomethingElseThrownByATest) {
  int suite_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> suite_Compare = [&](bool left, bool right) {
    suite_Compare_call_count++;
    return left == right;
  };
  int before_all_call_count = 0;
  MaybeTestConfigureFunction before_all = [&]() { before_all_call_count++; };
  int after_all_call_count = 0;
  MaybeTestConfigureFunction after_all = [&]() { after_all_call_count++; };
  int test_Compare_call_count = 0;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_call_count](bool left, bool right) {
    test_Compare_call_count++;
    return left == right;
  };
  int before_each_call_count = 0;
  MaybeTestConfigureFunction before_each = [&before_each_call_count]() { before_each_call_count++; };
  int after_each_call_count = 0;
  MaybeTestConfigureFunction after_each = [&after_each_call_count]() { after_each_call_count++; };
  int test_function_call_count = 0;
  function<bool()> test_function = [&test_function_call_count]() {
    test_function_call_count++;
    throw(42);
    return true;
  };

  // TODO: Remove this wrapper function once InterceptCout works properly with parameters.
  function<void()> wrapper = [&]() {
    ExecuteSuite("My Suite",
                 test_function,
                 {
                     MakeTest("Test Name", true, make_tuple(), test_Compare, before_each, after_each, true),
                 },
                 suite_Compare,
                 before_all,
                 after_all,
                 true);
  };

  string output = InterceptCout(wrapper);
  // string output = "";
  // EXPECT_THROW((output = InterceptCout(wrapper)), std::exception);
  EXPECT_THAT(output,
              Eq(
                  R"test(🚀Beginning Suite: My Suite
  Beginning Test: Test Name
    🔥ERROR: Caught something that is neither an std::exception nor an std::string.
    ❌FAILED: expected: 1, actual: 0
  Ending Test: Test Name
Ending Suite: My Suite
)test"));
  EXPECT_THAT(test_function_call_count, Eq(1));
  EXPECT_THAT(suite_Compare_call_count, Eq(0));
  EXPECT_THAT(before_all_call_count, Eq(1));
  EXPECT_THAT(after_all_call_count, Eq(1));
  EXPECT_THAT(test_Compare_call_count, Eq(1));
  EXPECT_THAT(before_each_call_count, Eq(1));
  EXPECT_THAT(after_each_call_count, Eq(1));
}

TEST(ExecuteSuiteWithTuple, ShouldNotExecuteADisabledSuite) {
  bool suite_Compare_called = false;
  MaybeTestCompareFunction<bool> suite_Compare = [&suite_Compare_called](bool left, bool right) {
    suite_Compare_called = true;
    return left == right;
  };
  bool before_all_called = false;
  MaybeTestConfigureFunction before_all = [&before_all_called]() { before_all_called = true; };
  bool after_all_called = false;
  MaybeTestConfigureFunction after_all = [&after_all_called]() { after_all_called = true; };
  bool test_Compare_called = false;
  MaybeTestCompareFunction<bool> test_Compare = [&test_Compare_called](bool left, bool right) {
    test_Compare_called = true;
    return left == right;
  };
  bool before_each_called = false;
  MaybeTestConfigureFunction before_each = [&before_each_called]() { before_each_called = true; };
  bool after_each_called = false;
  MaybeTestConfigureFunction after_each = [&after_each_called]() { after_each_called = true; };
  bool test_function_called = false;
  function<bool()> test_function = [&test_function_called]() {
    test_function_called = true;
    return true;
  };
  TestSuite<bool> test_suite = MakeTestSuite(
      "My Suite",
      []() { return true; },
      {
          MakeTest("Test Name", true, make_tuple(), test_Compare, before_each, after_each, true),
      },
      suite_Compare,
      before_all,
      after_all,
      false);
  function<void()> wrapper = [&test_suite]() { ExecuteSuite(test_suite); };

  string output = InterceptCout(wrapper);
  EXPECT_THAT(output,
              Eq("🚧Skipping suite: My Suite because it is disabled.\n  🚧Skipping Test: Test Name because the suite "
                 "is disabled.\n"));
  EXPECT_THAT(test_function_called, Eq(false));
  EXPECT_THAT(suite_Compare_called, Eq(false));
  EXPECT_THAT(before_all_called, Eq(false));
  EXPECT_THAT(after_all_called, Eq(false));
  EXPECT_THAT(test_Compare_called, Eq(false));
  EXPECT_THAT(before_each_called, Eq(false));
  EXPECT_THAT(after_each_called, Eq(false));
}

// TODO: Add tests for ExecuteSuite with tuple.
/*
For each ExecuteSuite variant.
- Should not execute a disabled suite.
* Should execute a suite with no tests.
* Should execute a suite with a single pass.
* Should execute a suite with a single fail.
* Should execute a suite with a single skip/disabled test.
* Should execute a suite with a pass and a disabled test.
* Should catch an exception thrown by a test and record it as an error.
* Should catch a string thrown by a test and record it as an error.
* Should catch something else thrown by a test and record it as an error.
For all tests
* Should call before_all once before the first before_each.
* Should call before_each once per test.
* Should call after_each once per test.
* Should call after_all once after the last after_each.
* Should print output. Test with one of each kind of test above to make sure all cout statements happen correctly.
Hijack cout before calling ExecuteSuite and restore it after. Split the recorded cout output into lines for easier
order checking while allowing for parallelism.
*/
// TODO: Test InterceptCout.
// TODO: Test container printer prints initializer_lists and other non-vector containers.
// TODO: Test SkipTest
}  // End namespace
