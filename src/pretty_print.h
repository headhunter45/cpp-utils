/**********************************************************************************************************************
 *
 * @file pretty_print.h
 *
 * @brief Declares function templates for printing objects in a friendlier manner.
 *
 * This specifically helps for printing stl containers, tuples, pairs, and queues as well as quoting strings
 * contained within them.
 *
 * @copyright Copyright (C) 2023 by Tom Hicks <headhunter3@gmail.com>
 *
 * Licensed under the MIT license see below for details.
 *
 **********************************************************************************************************************/
/*
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
#ifndef CPPUtils__pretty_print_h__
#define CPPUtils__pretty_print_h__
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <queue>
#include <regex>
#include <string>
#include <tuple>

namespace CPPUtils {
/// @addtogroup pretty_print Pretty Print
/// @{

/// @name Helper Templates
/// These are not directly tested.
/// @{

/// @brief This SFINAE struct is used to select the correct wide or narrow string based the TChar.
/// @tparam TChar The character type of the string we want. This should either be char or wchar_t.
template <typename TChar> struct StringTraits;

/// @brief This SFINAE struct is use to select a narrow string.
template <> struct StringTraits<char> {
  /// @brief Gets the narrow string.
  /// @param narrow The narrow string. This is always returned by this specialization.
  /// @param wide The wide string. This is ignored for this specialization.
  /// @return The narrow string.
  static constexpr const char *Literal(const char *narrow, const wchar_t *wide) { return narrow; }

  /// @brief Gets the narrow string.
  /// @param narrow The narrow string. This is always returned by this specialization.
  /// @param wide The wide string. This is ignored for this specialization.
  /// @return The narrow string.
  static const std::string Literal(const std::string &narrow, const std::wstring &wide) { return narrow; }
};

/// @brief This SFINAE struct is used to select a wide string.
template <> struct StringTraits<wchar_t> {
  /// @brief Gets the wide string.
  /// @param narrow The narrow string. This ignored for this specialization.
  /// @param wide The wide string. This is is always returned by this specialization.
  /// @return The wide string.
  static constexpr const wchar_t *Literal(const char *narrow, const wchar_t *wide) { return wide; }

  /// @brief Gets the wide string.
  /// @param narrow The narrow string. This ignored for this specialization.
  /// @param wide The wide string. This is is always returned by this specialization.
  /// @return The wide string.
  static const std::wstring Literal(const std::string &narrow, const std::wstring &wide) { return wide; }
};

/// @brief This SFINAE struct is used to help select container like types.
/// @tparam T The potential container type.
template <typename T> struct is_container {
  /// @brief Returns true because the type has begin and end methods.
  /// @tparam U The type to check.
  /// @param <anonymous> A pointer that is not used.
  /// @return True because the type is a container.
  template <typename U> static constexpr bool test(decltype(std::begin(std::declval<U>())) *) { return true; }

  /// @brief Returns false because the type is not a container.
  /// @tparam U The type to check.
  /// @param <anonymous> A pointer that is not used.
  /// @return False because the type is not a container.
  template <typename U> static constexpr bool test(...) { return false; }

  /// @brief Tests whether type T is a container.
  static constexpr bool value = test<T>(nullptr);
};

/// @}

/// @name Escape for Printing
/// These methods all escape different kinds of strings for printing. Currently they replace the ansi escape character
/// with a printable version of its code "\033".
/// @{

/// @brief This function escapes a char* or wchar_t* for printing to cout or wcout.
///
/// It replaces the escape character with a its code.
/// @tparam TChar The type of character of our string. This should be dertermined automatically based on the input type.
/// @param text The text to escape.
/// @return The escaped string.
template <typename TChar> std::basic_string<TChar> EscapeForPrinting(const TChar *text);

// std::string and std::wstring
/// @brief This function escapes a std::string or std::wstring for printing to cout or wcout.
/// @tparam TChar The type of character of our string. This should be dertermined automatically based on the input type.
/// @tparam TTraits The char_traits type of our string. This should be determined automatically based on the input type.
/// @param text The text to escape.
/// @return The escaped string.
template <typename TChar, typename TTraits>
std::basic_string<TChar, TTraits> EscapeForPrinting(const std::basic_string<TChar, TTraits> &text);

// std::string_view and std::wstring_view
/// @brief This function escapes a std::string_view or std::wstring_view for printing to cout or wcout.
/// @tparam TChar The type of character of our string. This should be dertermined automatically based on the input type.
/// @tparam TTraits The char_traits type of our string. This should be determined automatically based on the input type.
/// @param text The text to escape.
/// @return The escaped string.
template <typename TChar, typename TTraits>
std::basic_string<TChar, TTraits> EscapeForPrinting(const std::basic_string_view<TChar, TTraits> &text);
/// @}

/// @name  Pretty Print
/// These functions attempt to print friendlier versions of strings and objects that don't have operator<< defined for
/// them.
/// @{

/// @brief This function prints a pointer.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @param os The output stream to write to.
/// @param pointer The pointer to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const void *pointer);

/// @brief This function prints a const char* or const wchar_t* to the output stream.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @param os The output stream to write to.
/// @param text The text to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const TChar *text);

/// @brief This function prints a std::string or std::wstring to the output stream.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @param os The output stream to write to.
/// @param text The text to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const std::basic_string<TChar, TTraits> &text);

/// @brief This function prints a std::string_view or std::wstring_view to the output stream.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @param os The output stream to write to.
/// @param text The text to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const std::basic_string_view<TChar, TTraits> &text);

/// @brief This function prints a std::tuple to the output stream.
///
/// The output format for an empty tuple is "[]". For a tuple of (1, "two", 3.0) the output format is "[ 1, "two", 3.0
/// ]".
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @tparam ...TArgs The types of the tuple.
/// @param os The output stream to write to.
/// @param tuple The tuple to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits, typename... TArgs>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const std::tuple<TArgs...> &tuple);

/// @brief This function prints any stl-like container to the output stream.
///
/// It works with any class that has begin and end methods that return iterators.
///
/// The output format for an empty container is "[]". For a (vector<int>){1, 2, 3} the output format is "[ 1, 2, 3 ]".
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @tparam TContainer The type of the container to print.
/// @tparam Ignore this parameter it has a default value.
/// @tparam Ignore this parameter it has a default value.
/// @param os The output stream to write to.
/// @param container The container to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits, typename TContainer, typename, typename>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, TContainer container);

/// @brief This function prints an initializer list to the output stream.
///
/// The output format for a list {"hello", "world"} is "[ "hello", "world" ]". Unfortunately empty initializer lists are
/// treated as null pointers and will either print "null" or however your platform prints pointers.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @tparam TItem The type of the item in the initializer list.
/// @param os The output stream to write to.
/// @param list The initializer_list to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits, typename TItem>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, std::initializer_list<TItem> list);

/// @brief This function prints STL queues to the output stream.
///
/// The output format for an empty queue is "[]". The output format for a queue of 5,1,3 is "[ 5, 1, 3 ]".
///
/// The queue is copied and the copy is emptied.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @tparam TItem The type of the item in the initializer list.
/// @param os The output stream to write to.
/// @param queue The queue to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits, typename TItem>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, std::queue<TItem> queue);

/// @brief This function prints STL pairs to the output stream.
///
/// The output format for a pair of (3, "Kansas") is "(3, "Kansas")".
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @tparam TFirst The type of the first item in the pair.
/// @tparam TSecond The type of the second item in the pair.
/// @param os The output stream to write to.
/// @param value The pair to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits, typename TFirst, typename TSecond>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, std::pair<TFirst, TSecond> value);

/// @brief This function prints other items. It just pipes the item to the output stream.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @tparam TItem The type of the item to print.
/// @param os The output stream to write to.
/// @param item The item to print.
/// @return The output stream for chaining.
template <typename TChar,
          typename TTraits,
          typename TItem,
          typename std::enable_if<!is_container<TItem>::value>::type * = nullptr>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, TItem item);
/// @}

/// @name Pretty Print with Separator
/// These functions pretty print items with a separator.
/// @{

/// @brief This function prints varargs with a const char* or const wchar_t* separator between each pair.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @tparam ...Args The types of the arguments to print.
/// @param os The output stream to write to.
/// @param separator The separator to use between the items.
/// @param ...args The items to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits, typename... Args>
auto &PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits> &os, const TChar *separator, Args &&...args);

/// @brief This function prints varargs with a std::string or std::wstring separator between each pair.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @tparam ...Args The types of the arguments to print.
/// @param os The output stream to write to.
/// @param separator The separator to use between the items.
/// @param ...args The items to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits, typename... TArgs>
auto &PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits> &os,
                               std::basic_string<TChar, TTraits> separator,
                               TArgs &&...args);

/// @brief This function prints varargs with a std::string_view or std::wstring_view separator between each pair.
/// @tparam TChar The character type of our stream.
/// @tparam TTraits The character_traits type of our stream.
/// @tparam ...Args The types of the arguments to print.
/// @param os The output stream to write to.
/// @param separator The separator to use between the items.
/// @param ...args The items to print.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits, typename... TArgs>
auto &PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits> &os,
                               std::basic_string_view<TChar, TTraits> separator,
                               TArgs &&...args);

/// @}

// const char* and const wchar_t*
template <typename TChar> std::basic_string<TChar> EscapeForPrinting(const TChar *text) {
  std::regex regex = std::regex(StringTraits<TChar>::Literal("\033", L"\033"));
  const TChar *replace = StringTraits<TChar>::Literal("\\033", L"\\033");
  return std::regex_replace(text, regex, replace);
}

// std::string and std::wstring
template <typename TChar, typename TTraits>
std::basic_string<TChar, TTraits> EscapeForPrinting(const std::basic_string<TChar, TTraits> &text) {
  std::regex regex = std::regex(StringTraits<TChar>::Literal("\033", L"\033"));
  std::basic_string<TChar, TTraits> replace = StringTraits<TChar>::Literal("\\033", L"\\033");
  return std::regex_replace(text, regex, replace);
}

// std::string_view and std::wstring_view
template <typename TChar, typename TTraits>
std::basic_string<TChar, TTraits> EscapeForPrinting(const std::basic_string_view<TChar, TTraits> &text) {
  std::basic_string<TChar, TTraits> text_as_string = std::basic_string<TChar, TTraits>(text);
  std::regex regex = std::regex(StringTraits<TChar>::Literal("\033", L"\033"));
  std::basic_string<TChar, TTraits> replace = StringTraits<TChar>::Literal("\\033", L"\\033");
  return std::regex_replace(text_as_string, regex, replace);
}

// pointers
template <typename TChar, typename TTraits>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const void *pointer) {
  if (pointer == nullptr) {
    os << StringTraits<TChar>::Literal("null", L"null");
  } else {
    os << pointer;
  }
  return os;
}

// const char* and const wchar_t*
template <typename TChar, typename TTraits>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const TChar *item) {
  os << StringTraits<TChar>::Literal("\"", L"\"") << EscapeForPrinting(item)
     << StringTraits<TChar>::Literal("\"", L"\"");
  return os;
}

// std::string and std::wstring
template <typename TChar, typename TTraits>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const std::basic_string<TChar, TTraits> &text) {
  os << StringTraits<TChar>::Literal("\"", L"\"") << EscapeForPrinting(text)
     << StringTraits<TChar>::Literal("\"", L"\"");
  return os;
}

// std::string_view and std::wstring_view
template <typename TChar, typename TTraits>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const std::basic_string_view<TChar, TTraits> &text) {
  os << StringTraits<TChar>::Literal("\"", L"\"") << EscapeForPrinting(text)
     << StringTraits<TChar>::Literal("\"", L"\"");
  return os;
}

// std::tuple
template <typename TChar, typename TTraits, typename... TArgs>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, const std::tuple<TArgs...> &tuple) {
  std::apply(
      [&os](auto &&...args) {
        if (sizeof...(TArgs) == 0) {
          os << StringTraits<TChar>::Literal("[]", L"[]]");
          return;
        }
        size_t n = 0;
        os << StringTraits<TChar>::Literal("[ ", L"[ ");
        ((PrettyPrint(os, args) << (++n != sizeof...(TArgs) ? StringTraits<TChar>::Literal(", ", L", ")
                                                            : StringTraits<TChar>::Literal("", L""))),
         ...);
        os << StringTraits<TChar>::Literal(" ]", L" ]");
      },
      tuple);
  return os;
}

// STL containers
template <typename TChar,
          typename TTraits,
          typename TContainer,
          typename = decltype(std::begin(std::declval<TContainer>())),
          typename = decltype(std::end(std::declval<TContainer>())),
          typename = typename TContainer::value_type>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, TContainer container) {
  if (container.size() <= 0) {
    os << StringTraits<TChar>::Literal("[]", L"[]");
  } else {
    os << StringTraits<TChar>::Literal("[ ", L"[ ");
    for (auto it = std::begin(container); it != std::end(container); it++) {
      if (it != std::begin(container)) {
        os << StringTraits<TChar>::Literal(", ", L", ");
      }
      PrettyPrint(os, *it);
    }
    os << StringTraits<TChar>::Literal(" ]", L" ]");
  }
  return os;
}

// std::initializer_list
template <typename TChar, typename TTraits, typename TItem>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, std::initializer_list<TItem> list) {
  if (list.size() <= 0) {
    os << StringTraits<TChar>::Literal("[]", L"[]");
    return os;
  }
  os << StringTraits<TChar>::Literal("[ ", L"[ ");
  for (auto it = std::begin(list); it != std::end(list); it++) {
    if (it != std::begin(list)) {
      os << StringTraits<TChar>::Literal(", ", L", ");
    }
    PrettyPrint(os, *it);
  }
  os << StringTraits<TChar>::Literal(" ]", L" ]");
  return os;
}

// std::queue
template <typename TChar, typename TTraits, typename TItem>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, std::queue<TItem> queue) {
  if (queue.empty()) {
    os << StringTraits<TChar>::Literal("[]", L"[]");
    return os;
  }
  os << StringTraits<TChar>::Literal("[ ", L"[ ");
  PrettyPrint(os, queue.front());
  queue.pop();
  while (!queue.empty()) {
    os << StringTraits<TChar>::Literal(", ", L", ");
    PrettyPrint(os, queue.front());
    queue.pop();
  }
  os << StringTraits<TChar>::Literal(" ]", L" ]");
  return os;
}

// std::pair
template <typename TChar, typename TTraits, typename TFirst, typename TSecond>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, std::pair<TFirst, TSecond> value) {
  os << StringTraits<TChar>::Literal("(", L"(");
  PrettyPrint(os, value.first);
  os << StringTraits<TChar>::Literal(", ", L", ");
  PrettyPrint(os, value.second);
  os << StringTraits<TChar>::Literal(")", L")");
  return os;
}

// Catch-all for everything else.
template <typename TChar,
          typename TTraits,
          typename TItem,
          typename std::enable_if<!is_container<TItem>::value>::type *>
auto &PrettyPrint(std::basic_ostream<TChar, TTraits> &os, TItem item) {
  os << item;
  return os;
}

// Prints args with separator between them. const char* separator.
template <typename TChar, typename TTraits, typename... TArgs>
auto &PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits> &os, const TChar *separator, TArgs &&...args) {
  if (sizeof...(TArgs) == 0) {
    os << StringTraits<TChar>::Literal("", L"");
    return os;
  }
  size_t n = 0;
  ((PrettyPrint(os, args) << (++n != sizeof...(TArgs) ? separator : StringTraits<TChar>::Literal("", L""))), ...);
  return os;
}

// Prints args with separator between them. std::string separator.
template <typename TChar, typename TTraits, typename... TArgs>
auto &PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits> &os,
                               std::basic_string<TChar, TTraits> separator,
                               TArgs &&...args) {
  if (sizeof...(TArgs) == 0) {
    os << StringTraits<TChar>::Literal("", L"");
    return os;
  }
  size_t n = 0;
  ((PrettyPrint(os, args) << (++n != sizeof...(TArgs) ? separator : StringTraits<TChar>::Literal("", L""))), ...);
  return os;
}

// Prints args with separator between them. std::string_view separator.
template <typename TChar, typename TTraits, typename... TArgs>
auto &PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits> &os,
                               std::basic_string_view<TChar, TTraits> separator,
                               TArgs &&...args) {
  if (sizeof...(TArgs) == 0) {
    os << StringTraits<TChar>::Literal("", L"");
    return os;
  }
  size_t n = 0;
  ((PrettyPrint(os, args) << (++n != sizeof...(TArgs) ? separator : StringTraits<TChar>::Literal("", L""))), ...);
  return os;
}

/// @}
} // End namespace CPPUtils

#endif // End !defined CPPUtils__pretty_print_h__
