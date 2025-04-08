#ifndef TinyTest__tinytest_h__
#define TinyTest__tinytest_h__
/***************************************************************************************
 * @file tinytest.h                                                                    *
 *                                                                                     *
 * @brief Defines structs and functions for implementing TinyTest.                     *
 * @copyright Copyright 2023 Tom Hicks <headhunter3@gmail.com>                         *
 * Licensed under the MIT license see the LICENSE file for details.                    *
 ***************************************************************************************/

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "pretty_print.h"

namespace TinyTest {

/// @defgroup tests Tests
/// @defgroup test_suites Test Suites
/// @defgroup test_results Test Results
/// @defgroup test_execution Test Execution
/// @defgroup configure_functions Configure Functions
/// @defgroup compare_functions Compare Functions
/// @defgroup helpers Helpers

/// @addtogroup configure_functions
/// @{

/// @brief This is a type that represents a setup or teardown function for tests.
using TestConfigureFunction = std::function<void()>;

/// @brief This is a type that represents an optional setup or teardown function for tests.
using MaybeTestConfigureFunction = std::optional<TestConfigureFunction>;

/// @brief This is the default configure function.
/// @return The default configure function. This is currently std::nullopt.
MaybeTestConfigureFunction DefaultTestConfigureFunction();

/// @brief Combines multiple test configure functions into a single one.
/// @param first The first setup function if this is nullopt it is ignored.
/// @param second The second setup function if this is nullopt it is ignored.
/// @return The resulting setup function or nullopt if both first and second are nullopt.
MaybeTestConfigureFunction Coalesce(MaybeTestConfigureFunction first, MaybeTestConfigureFunction second);
/// @}

/// @addtogroup compare_functions
/// @{

/// @brief This is a type that represents a compare function for types of TResult.
///
/// It should return true if the parameters are equal and false otherwise.
/// @tparam TResult The type of the parameters. This is the return type of the function being tested.
template <typename TResult>
using TestCompareFunction = std::function<bool(const TResult& expected, const TResult& actual)>;

/// @brief This is a type that represents an optional compare function for types of TResult.
/// @tparam TResult The type of the parameters. This is the return type of the function being tested.
template <typename TResult>
using MaybeTestCompareFunction = std::optional<TestCompareFunction<TResult>>;

/// @brief This is the default compare function.
/// @tparam TResult The return type of the test function.
/// @return The default compare function. Currently this is std::nullopt.
template <typename TResult>
MaybeTestCompareFunction<TResult> DefaultTestCompareFunction();
/// @}

// TODO: For some reason all hell breaks loose if test_name or expected output
// are const&. Figure out why. Probably need to use decay and make const& where we want it explicitly.
/// @addtogroup tests
/// @{

/// @brief This is a type that represents an individual test.
/// @tparam TResult The return type of the test function.
/// @tparam ...TInputParams The parameters to pass to the test function
template <typename TResult, typename... TInputParams>
using TestTuple = std::tuple<
    /// test_name
    std::string,
    /// expected_output
    TResult,
    /// input_params - The input parameters for this test. These will be used when calling std::apply with
    /// function_to_test to execute the test.
    std::tuple<TInputParams...>,
    /// test_compare_function - If this is not nullptr then this function will be called instead of
    /// suite_compare_function to determine if the test passes. Use this to check for side effects of the test. Return
    /// true if the test passes and false otherwise.
    MaybeTestCompareFunction<TResult>,
    /// test_setup_function - If this is not nullptr this function is called before each test to setup the environment.
    /// It is called with std::apply and input_params so you can use them to mock records with specific IDs or calculate
    /// an expected result.
    MaybeTestConfigureFunction,
    /// test_teardown_function - If this is not nullptr this function is called after each test to cleanup any
    /// allocated/shared resources.
    MaybeTestConfigureFunction,
    /// is_enabled - If this is false the test, setup, and teardown functions are not run.
    bool>;

/// @brief Executes a TestSuite.
/// @tparam TResult The result type of the test.
/// @tparam ...TInputParams The types of parameters sent to the test function.
/// @param test_name The label for this test. For example "should calculate the
/// interest".
/// @param expected The expected output of calling the test function with these
/// input parameters.
/// @param input_params The input parameters to use when calling the test
/// function.
/// @param test_compare_function An optional function that can be used to Compare the
/// expected and actual return values. This is good for when you only care about
/// certain fields being equal.
/// @param before_each This is called to setup the environment before running
/// the test. This is where you should build mocks, setup spies, and set any
/// other values you need before calling the test function.
/// @param after_each This is called after each test run to cleanup anything
/// allocated in before_each.
/// @param is_enabled If false this test run is not executed and considered
/// skipped for reporting purposes.
/// @return A TestTuple suitable for use as a test run when calling test_fn.
template <typename TResult, typename... TInputParams>
TestTuple<TResult, TInputParams...> MakeTest(const std::string& test_name,
                                             const TResult& expected,
                                             std::tuple<TInputParams...> input_params,
                                             MaybeTestCompareFunction<TResult> test_compare_function = std::nullopt,
                                             MaybeTestConfigureFunction before_each = std::nullopt,
                                             MaybeTestConfigureFunction after_each = std::nullopt,
                                             bool is_enabled = true);
/// @}

/// @addtogroup test_suites
/// @{

/// @brief This type represents a test suite.
/// @tparam TResult The return type of the function to test.
/// @tparam TFunctionToTest The type of the function to test.
/// @tparam ...TInputParams The types of the input parameters to the function to test.
template <typename TResult, typename... TInputParams>
using TestSuite = std::tuple<
    /// test_name - The name of the test.
    std::string,
    /// function_to_test - The function to test. It will be executed once for each item in the tests initializer_list.
    std::function<TResult(TInputParams...)>,
    /// tests - This is an initializer list of @link TestTuple @endlink that represent the test runs to execute.
    std::initializer_list<TestTuple<TResult, TInputParams...>>,
    /// test_compare_function - This is an optional function that overrides how test results are compared.
    MaybeTestCompareFunction<TResult>,
    /// before_each - This is an optional function that is executed before each test.
    MaybeTestConfigureFunction,
    /// after_each - This is an optional function that is executed after each test.
    MaybeTestConfigureFunction,
    // is_enabled - If true the suite is executed. If false all test runs are reported as skipped and none are run.
    bool>;

/// @brief Makes a TestSuite tuple from the given parameters.
/// @tparam TResult The return type of function_to_test.
/// @tparam TFunctionToTest The type of function_to_test.
/// @tparam ...TInputParams The parameter types of function_to_test.
/// @param suite_name The label for this test suite.
/// @param function_to_test The function to test.
/// @param test_data The configuration for the test runs.
/// @param compare An optional compare function to use when evaluating test results.
/// @param before_each An optional function to run before each test.
/// @param after_each An optional function to run after each test.
/// @param is_enabled If false the test suite is skipped. All tests in the suite will be reported as skipped.
/// @return The results of the test suite.
template <typename TResult, typename TFunctionToTest, typename... TInputParams>
TestSuite<TResult, TInputParams...> MakeTestSuite(const std::string& suite_name,
                                                  TFunctionToTest function_to_test,
                                                  std::initializer_list<TestTuple<TResult, TInputParams...>> test_data,
                                                  MaybeTestCompareFunction<TResult> compare = std::nullopt,
                                                  MaybeTestConfigureFunction before_each = std::nullopt,
                                                  MaybeTestConfigureFunction after_each = std::nullopt,
                                                  bool is_enabled = true);
/// @}

/// @addtogroup helpers
/// @{

/// @brief Intercepts cout, executes the function and returns a string with all tesxt sent to cout while running the
/// function.
/// @tparam TResult The return type of function_to_execute.
/// @tparam ...TParameters The parameter types of function_to_execute.
/// @param function_to_execute The function to execute.
/// @param maybe_args The args to call function_to_execute with or nullopt if it takes no parameters.
/// @return A string containing all text written to cout by function_to_execute.
template <typename TResult, typename... TParameters>
std::string InterceptCout(std::function<TResult(TParameters...)> function_to_execute,
                          std::optional<std::tuple<TParameters...>> maybe_args = std::nullopt);

/// @brief This function compares two vectors.
/// @tparam TChar The character type of the stream to write to.
/// @tparam TTraits The character_traits type of the stream to write to.
/// @tparam TItem The type of item in the vectors.
/// @param error_message The stream to write error messages to.
/// @param expected The expected vector.
/// @param actual The actual vector.
/// @return The error_message stream.
template <typename TChar, typename TTraits, typename TItem>
auto& Compare(std::basic_ostream<TChar, TTraits>& error_message,
              std::vector<TItem> expected,
              std::vector<TItem> actual);

/// @}

/// @addtogroup test_results

/// @brief Represents the results of running some number of tests.
///
/// This type may evolve over time, but currently it tracks:
/// * The total number of tests run.
/// * The number of failures and any messages sent with the failures.
/// * The number of tests skipped and any messages sent with the skips.
/// * The number of tests with errors and any error messages sent with the errors.
///    * __Note:__ Errors do not count as test runs. Errored tests will be recorded as a passed/failed/skipped test
///    separately.
/// * The number of passed tests.
class TestResults {
 public:
  /// @brief Creates an empty TestResults instance representing no tests run.
  TestResults();

  /// @brief Creates a new TestResults instance that is a copy of other.
  /// @param other
  TestResults(const TestResults& other);

  /// @brief Creates a new TestResults instance with specific counts.
  /// @param errors The number of errors while running the tests.
  /// @param failed The number of failed tests.
  /// @param passed The number of passed tests.
  /// @param skipped The number of skipped tests.
  /// @param total The total number of tests run. This should equal the sum of failed, passed, and skipped tests.
  /// @param error_messages The list of error messages.
  /// @param failure_messages The list of failure messages.
  /// @param skip_messages The list of skip messages.
  TestResults(uint32_t errors,
              uint32_t failed,
              uint32_t passed,
              uint32_t skipped,
              uint32_t total,
              std::vector<std::string> error_messages,
              std::vector<std::string> failure_messages,
              std::vector<std::string> skip_messages);

  /// @brief Adds an error. This increments errors.
  /// @return A reference to this instance. Used for chaining.
  TestResults& Error();

  /// @brief Adds an error with a message. This increments errors as well as
  /// saving the error message.
  /// @param message The error message.
  /// @return A reference to this instance. Used for chaining.
  TestResults& Error(std::string message);

  /// @brief Adds a failed test. This increments total and failed.
  /// @return A reference to this instance. Used for chaining.
  TestResults& Fail();

  /// @brief Adds a failed test with a message. This increments total and failed
  /// as well as saving the failure message.
  /// @param message The reason the test failed.
  /// @return A reference to this instance. Used for chaining.
  TestResults& Fail(const std::string& message);

  /// @brief Adds a passed test. This increments total and passed.
  /// @return A reference to this instance. Used for chaining.
  TestResults& Pass();

  /// @brief Adds a skipped test. This increments total and skipped.
  /// @return A reference to this instance. Used for chaining.
  TestResults& Skip();

  /// @brief Adds a skipped test with a message. This increments total and
  /// skipped as well as saving the skip message.
  /// @param message The reason the test was skipped.
  /// @return A reference to this instance. Used for chaining.
  TestResults& Skip(const std::string& message);

  /// @brief Getter for the list of error messages.
  /// @return
  std::vector<std::string> ErrorMessages() const;

  /// @brief Getter for the count of errors.
  /// @return
  uint32_t Errors() const;

  /// @brief Getter for the count of failed tests.
  /// @return The count of failed tests.
  uint32_t Failed() const;

  /// @brief Getter for the list of failure messages.
  /// @return The list of failure messages.
  std::vector<std::string> FailureMessages() const;

  /// @brief Getter for the count of passed tests.
  /// @return The count of passed tests.
  uint32_t Passed() const;

  /// @brief Getter for the count of skipped tests.
  /// @return The count of skipped tests.
  uint32_t Skipped() const;

  /// @brief Getter for the list of skip messages.
  /// @return The list of skip messages.
  std::vector<std::string> SkipMessages() const;

  /// @brief Getter for the count of total tests.
  /// @return The count of total tests run.
  uint32_t Total() const;

  /// @brief Returns the combination of this and another TestResults instance.
  /// @param other The other TestResults instance to add to this one.
  /// @return The combination of the two TestResults instances.
  TestResults operator+(const TestResults& other) const;

  /// @brief Adds another TestResults to this one and returns a reference to
  /// this instance.
  /// @param other The other TestResults instance to add to this one.
  /// @return A reference to this instance.
  TestResults& operator+=(const TestResults& other);

 private:
  std::vector<std::string> error_messages_;
  uint32_t errors_;
  uint32_t failed_;
  std::vector<std::string> failure_messages_;
  uint32_t passed_;
  std::vector<std::string> skip_messages_;
  uint32_t skipped_;
  uint32_t total_;
};

/// @brief Writes a friendly version of results to the provided stream.
/// @param os The stream to write to.
/// @param results The TestResults to write.
void PrintResults(std::ostream& os, TestResults results);

/// @addtogroup test_execution
/// @{

/// @brief This function marks a test as skipped with an optional reason.
/// @param results The TestResults to update.
/// @param suite_label The label for the test suite.
/// @param test_label The label for the test.
/// @param reason The optional reason the test is being skipped.
/// @return The TestResults for chaining.
TestResults& SkipTest(TestResults& results,
                      const std::string& suite_label,
                      const std::string& test_label,
                      std::optional<const std::string> reason = std::nullopt);

/// @brief Executes a TestSuite.
/// @tparam TResult The result type of the test.
/// @tparam TInputParams... The types of parameters sent to the test function.
/// @param suite_label The label for this test suite. For example a class name
/// such as "MortgageCalculator".
/// @param function_to_test The function to be tested. It will be called with
/// std::apply and a std::tuple<TInputParams...> made from each item in tests.
/// @param tests An std::initializer_list of test runs.
/// @param suite_Compare A function used to Compare the expected and actual test
/// results. This can be overridden per test by setting test_Compare.
/// @param after_all This is called before each suite is started to setup the
/// environment. This is where you should build mocks, setup spies, and test
/// fixtures.
/// @param before_all This is called after each suite has completed to cleanup
/// anything allocated in suite_before_each.
/// @param is_enabled If false the test is reported as skipped. If true the test
/// is run as normal.
template <typename TResult, typename... TInputParams>
TestResults ExecuteSuite(std::string suite_label,
                         std::function<TResult(TInputParams...)> function_to_test,
                         std::initializer_list<TestTuple<TResult, TInputParams...>> tests,
                         MaybeTestCompareFunction<TResult> suite_Compare = std::nullopt,
                         MaybeTestConfigureFunction before_all = std::nullopt,
                         MaybeTestConfigureFunction after_all = std::nullopt,
                         bool is_enabled = true);

/// @brief
/// @tparam TResult The result type of the test.
/// @tparam TInputParams... The types of parameters sent to the test function.
/// @param test_suite A tuple representing the test suite configuration.
template <typename TResult, typename... TInputParams>
TestResults ExecuteSuite(const TestSuite<TResult, TInputParams...>& test_suite);

/// @}

template <typename TResult>
MaybeTestCompareFunction<TResult> DefaultTestCompareFunction() {
  return std::nullopt;
}

template <typename TResult, typename... TInputParams>
TestTuple<TResult, TInputParams...> MakeTest(const std::string& test_name,
                                             const TResult& expected,
                                             std::tuple<TInputParams...> input_params,
                                             MaybeTestCompareFunction<TResult> test_compare_function,
                                             MaybeTestConfigureFunction before_each,
                                             MaybeTestConfigureFunction after_each,
                                             bool is_enabled) {
  return make_tuple(test_name, expected, input_params, test_compare_function, before_each, after_each, is_enabled);
}

template <typename TResult, typename TFunctionToTest, typename... TInputParams>
TestSuite<TResult, TInputParams...> MakeTestSuite(const std::string& suite_name,
                                                  TFunctionToTest function_to_test,
                                                  std::initializer_list<TestTuple<TResult, TInputParams...>> test_data,
                                                  MaybeTestCompareFunction<TResult> compare,
                                                  MaybeTestConfigureFunction before_each,
                                                  MaybeTestConfigureFunction after_each,
                                                  bool is_enabled) {
  return make_tuple(suite_name, function_to_test, test_data, compare, before_each, after_each, is_enabled);
}

template <typename TResult, typename... TParameters>
std::string InterceptCout(std::function<TResult(TParameters...)> function_to_execute,
                          std::optional<std::tuple<TParameters...>> maybe_args) {
  std::ostringstream os;
  auto saved_buffer = std::cout.rdbuf();
  std::cout.rdbuf(os.rdbuf());

  if (maybe_args.has_value()) {
    std::apply(function_to_execute, maybe_args.value());
  } else {
    std::invoke(function_to_execute);
  }
  std::cout.rdbuf(saved_buffer);
  return os.str();
}

template <typename TChar, typename TTraits, typename TItem>
auto& Compare(std::basic_ostream<TChar, TTraits>& error_message,
              std::vector<TItem> expected,
              std::vector<TItem> actual) {
  if (expected.size() != actual.size()) {
    error_message << "size mismatch expected: " << expected.size() << ", actual: " << actual.size();
    return error_message;
  }

  for (size_t index = 0; index < expected.size(); index++) {
    if (expected[index] != actual[index]) {
      error_message << "vectors differ at index " << index << ", ";
      CPPUtils::PrettyPrint(error_message, expected[index]) << " != ";
      CPPUtils::PrettyPrint(error_message, actual[index]) << ", expected: ";
      CPPUtils::PrettyPrint(error_message, expected) << ", actual: ";
      CPPUtils::PrettyPrint(error_message, actual);
      return error_message;
    }
  }
  return error_message;
}

template <typename TResult, typename... TInputParams>
TestResults ExecuteSuite(std::string suite_label,
                         std::function<TResult(TInputParams...)> function_to_test,
                         std::initializer_list<TestTuple<TResult, TInputParams...>> tests,
                         MaybeTestCompareFunction<TResult> suite_Compare,
                         MaybeTestConfigureFunction before_all,
                         MaybeTestConfigureFunction after_all,
                         bool is_enabled) {
  TestResults results;
  if (!is_enabled) {
    std::cout << "🚧Skipping suite: " << suite_label << " because it is disabled." << std::endl;
    for (auto test : tests) {
      std::string test_label = std::get<0>(test);
      SkipTest(results, suite_label, test_label, "the suite is disabled.");
    }
    return results;
  }
  if (tests.size() == 0) {
    std::cout << "🚧Skipping suite: " << suite_label << " because it is empty." << std::endl;
    return results;
  }
  std::cout << "🚀Beginning Suite: " << suite_label << std::endl;

  // Step 1: Suite Setup

  if (before_all.has_value()) {
    (*before_all)();
  }

  // Step 2: Execute Tests
  for_each(tests.begin(),
           tests.end(),
           [&suite_label, &function_to_test, &results, &suite_Compare](TestTuple<TResult, TInputParams...> test_data) {
             // Step 2a: Extract our variables from the TestTuple.
             const std::string& test_label = std::get<0>(test_data);
             const std::string qualified_test_label = suite_label + "::" + test_label;
             const TResult& expected_output = std::get<1>(test_data);
             std::tuple<TInputParams...> input_params = std::get<2>(test_data);
             MaybeTestCompareFunction<TResult> maybe_Compare_function = std::get<3>(test_data);
             TestCompareFunction<TResult> Compare_function =
                 maybe_Compare_function.has_value() ? *maybe_Compare_function
                 : suite_Compare.has_value()        ? *suite_Compare
                                                    : [](const TResult& l, const TResult& r) { return l == r; };
             MaybeTestConfigureFunction before_each = std::get<4>(test_data);
             MaybeTestConfigureFunction after_each = std::get<5>(test_data);
             bool is_enabled = std::get<6>(test_data);

             if (!is_enabled) {
               SkipTest(results, suite_label, test_label);
               return;
             }

             // Step 2b: Test Setup
             std::cout << "  Beginning Test: " << test_label << std::endl;
             if (before_each.has_value()) {
               (*before_each)();
             }

             TResult actual;
             try {
               // Step 2c: Execute the test method.
               actual = std::apply(function_to_test, input_params);
             } catch (const std::exception& ex) {
               std::ostringstream os;
               os << "Caught exception \"" << ex.what() << "\".";
               results.Error(qualified_test_label + " " + os.str());
               std::cout << "    🔥ERROR: " << os.str() << std::endl;
             } catch (const std::string& message) {
               std::ostringstream os;
               os << "Caught string \"" << message << "\".";
               results.Error(qualified_test_label + " " + os.str());
               std::cout << "    🔥ERROR: " << os.str() << std::endl;
             } catch (const char* message) {
               std::ostringstream os;
               os << "Caught c-string \"" << message << "\".";
               results.Error(qualified_test_label + " " + os.str());
               std::cout << "    🔥ERROR: " << os.str() << std::endl;
             } catch (...) {
               std::string message =
                   "Caught something that is neither an std::exception "
                   "nor an std::string.";
               results.Error(qualified_test_label + " " + message);
               std::cout << "    🔥ERROR: " << message << std::endl;
             }

             // Step 2d: Pass or fail.
             if (Compare_function(expected_output, actual)) {
               results.Pass();
               std::cout << "    ✅PASSED" << std::endl;
             } else {
               std::ostringstream os;
               os << "expected: ";
               CPPUtils::PrettyPrint(os, expected_output) << ", actual: ";
               CPPUtils::PrettyPrint(os, actual);
               results.Fail(qualified_test_label + " " + os.str());
               std::cout << "    ❌FAILED: " << os.str() << std::endl;
             }

             // Step 2e: Test Teardown
             if (after_each.has_value()) {
               (*after_each)();
             }
             std::cout << "  Ending Test: " << test_label << std::endl;
           });

  // Step 3: Suite Teardown
  if (after_all.has_value()) {
    (*after_all)();
  }
  std::cout << "Ending Suite: " << suite_label << std::endl;
  return results;
}

template <typename TResult, typename... TInputParams>
TestResults ExecuteSuite(const TestSuite<TResult, TInputParams...>& test_suite) {
  std::string suite_label = std::get<0>(test_suite);
  std::function<TResult(TInputParams...)> function_to_test = std::get<1>(test_suite);
  std::initializer_list<TestTuple<TResult, TInputParams...>> tests = std::get<2>(test_suite);
  MaybeTestCompareFunction<TResult> suite_Compare = sizeof(test_suite) > 3 ? std::get<3>(test_suite) : std::nullopt;
  MaybeTestConfigureFunction before_all = sizeof(test_suite) > 4 ? std::get<4>(test_suite) : std::nullopt;
  MaybeTestConfigureFunction after_all = sizeof(test_suite) > 5 ? std::get<5>(test_suite) : std::nullopt;
  bool is_enabled = sizeof(test_suite) > 6 ? std::get<6>(test_suite) : true;
  return ExecuteSuite(suite_label, function_to_test, tests, suite_Compare, before_all, after_all, is_enabled);
}

}  // End namespace TinyTest

#endif  // End !defined(TinyTest__tinytest_h__)
