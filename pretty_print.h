#ifndef CPP_UTILS_pretty_print_h__
#define CPP_UTILS_pretty_print_h__
/***************************************************************************************
 * @file pretty_print.cpp                                                              *
 *                                                                                     *
 * @brief Defines functions for printing objects in a frendlier manner.                *
 * @copyright                                                                          *
 * Copyright 2023 Tom Hicks                                                            *
 * Licensed under the MIT license see the LICENSE file for details.                    *
 ***************************************************************************************/
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <queue>
#include <regex>
#include <string>
#include <tuple>

/** \addtogroup Pretty Print
 * @{
 */
namespace CPPUtils {
////////////////////////////////////////////////////////////////////////////////
// Helper templates - These are not directly tested.
template <typename TChar>
struct StringTraits;

template <>
struct StringTraits<char> {
  static constexpr const char* Literal(const char* narrow, const wchar_t* wide) { return narrow; }
};

template <>
struct StringTraits<wchar_t> {
  static constexpr const wchar_t* Literal(const char* narrow, const wchar_t* wide) { return wide; }
};

template <typename T>
struct is_container {
  template <typename U>
  static constexpr bool test(decltype(std::begin(std::declval<U>()))*) {
    return true;
  }

  template <typename U>
  static constexpr bool test(...) {
    return false;
  }

  static constexpr bool value = test<T>(nullptr);
};

////////////////////////////////////////////////////////////////////////////////
// Forward Declarations

// const char* and const wchar_t*
template <typename TChar>
std::basic_string<TChar> EscapeForPrinting(const TChar* text);

// std::string and std::wstring
template <typename TChar, typename TTraits>
std::basic_string<TChar, TTraits> EscapeForPrinting(const std::basic_string<TChar, TTraits>& text);

// std::string_view and std::wstring_view
template <typename TChar, typename TTraits>
std::basic_string<TChar, TTraits> EscapeForPrinting(const std::basic_string_view<TChar, TTraits>& text);

// pointers
template <typename TChar, typename TTraits>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const void* pointer);

// const char* and const wchar_t*
template <typename TChar, typename TTraits>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const TChar* item);

// std::string and std::wstring
template <typename TChar, typename TTraits>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const std::basic_string<TChar, TTraits>& item);

// std::string_view and std::wstring_view
template <typename TChar, typename TTraits>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const std::basic_string_view<TChar, TTraits>& item);

// std::tuple
template <typename TChar, typename TTraits, typename... TArgs>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const std::tuple<TArgs...>& tuple);

// Containers (anything with a begin and end iterator)
template <typename TChar, typename TTraits, typename TContainer, typename, typename>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, TContainer container);

// Initializer Lists
template <typename TChar, typename TTraits, typename TItem>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, std::initializer_list<TItem> container);

// queues
template <typename TChar, typename TTraits, typename TItem>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, std::queue<TItem> queue);

// pairs
template <typename TChar, typename TTraits, typename TFirst, typename TSecond>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, std::pair<TFirst, TSecond> value);

// Catch-all for everything else. Just print it and hope that works.
template <typename TChar,
          typename TTraits,
          typename TItem,
          typename std::enable_if<!is_container<TItem>::value>::type* = nullptr>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, TItem item);

// Prints varargs with a const char* or const wchar_t* separator between each pair.
template <typename TChar, typename TTraits, typename... Args>
auto& PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits>& os, const TChar* separator, Args&&... args);

// Prints varargs with a std::string or std::wstring separator.
template <typename TChar, typename TTraits, typename... TArgs>
auto& PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits>& os,
                               std::basic_string<TChar, TTraits> separator,
                               TArgs&&... args);

// Prints varargs with a std::string_view or std::wstring_view separator.
template <typename TChar, typename TTraits, typename... TArgs>
auto& PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits>& os,
                               std::basic_string_view<TChar, TTraits> separator,
                               TArgs&&... args);

////////////////////////////////////////////////////////////////////////////////
// Actual implementations

// const char*
template <typename TChar>
std::basic_string<TChar> EscapeForPrinting(const TChar* text) {
  std::regex regex = std::regex(StringTraits<TChar>::Literal("\033", L"\033"));
  const TChar* replace = StringTraits<TChar>::Literal("\\033", L"\\033");
  return std::regex_replace(text, regex, replace);
}

// std::string
template <typename TChar, typename TTraits>
std::basic_string<TChar, TTraits> EscapeForPrinting(const std::basic_string<TChar, TTraits>& text) {
  std::regex regex = std::regex(StringTraits<TChar>::Literal("\033", L"\033"));
  std::basic_string<TChar, TTraits> replace = StringTraits<TChar>::Literal("\\033", L"\\033");
  return std::regex_replace(text, regex, replace);
}

// std::string_view
template <typename TChar, typename TTraits>
std::basic_string<TChar, TTraits> EscapeForPrinting(const std::basic_string_view<TChar, TTraits>& text) {
  std::basic_string<TChar, TTraits> text_as_string = std::basic_string<TChar, TTraits>(text);
  std::regex regex = std::regex(StringTraits<TChar>::Literal("\033", L"\033"));
  std::basic_string<TChar, TTraits> replace = StringTraits<TChar>::Literal("\\033", L"\\033");
  return std::regex_replace(text_as_string, regex, replace);
}

// pointers
template <typename TChar, typename TTraits>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const void* pointer) {
  if (pointer == nullptr) {
    os << StringTraits<TChar>::Literal("null", L"null");
  } else {
    os << pointer;
  }
  return os;
}

// const char*.
template <typename TChar, typename TTraits>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const TChar* item) {
  os << StringTraits<TChar>::Literal("\"", L"\"") << EscapeForPrinting(item)
     << StringTraits<TChar>::Literal("\"", L"\"");
  return os;
}

// std::string.
template <typename TChar, typename TTraits>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const std::basic_string<TChar, TTraits>& item) {
  os << StringTraits<TChar>::Literal("\"", L"\"") << EscapeForPrinting(item)
     << StringTraits<TChar>::Literal("\"", L"\"");
  return os;
}

// std::string_view.
template <typename TChar, typename TTraits>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const std::basic_string_view<TChar, TTraits>& item) {
  os << StringTraits<TChar>::Literal("\"", L"\"") << EscapeForPrinting(item)
     << StringTraits<TChar>::Literal("\"", L"\"");
  return os;
}

// std::tuple
template <typename TChar, typename TTraits, typename... TArgs>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, const std::tuple<TArgs...>& tuple) {
  std::apply(
      [&os](auto&&... args) {
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

// Containers
template <typename TChar,
          typename TTraits,
          typename TContainer,
          typename = decltype(std::begin(std::declval<TContainer>())),
          typename = decltype(std::end(std::declval<TContainer>())),
          typename = typename TContainer::value_type>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, TContainer container) {
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

// initializer_lists
template <typename TChar, typename TTraits, typename TItem>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, std::initializer_list<TItem> container) {
  if (container.size() <= 0) {
    os << StringTraits<TChar>::Literal("[]", L"[]");
    return os;
  }
  os << StringTraits<TChar>::Literal("[ ", L"[ ");
  for (auto it = std::begin(container); it != std::end(container); it++) {
    if (it != std::begin(container)) {
      os << StringTraits<TChar>::Literal(", ", L", ");
    }
    PrettyPrint(os, *it);
  }
  os << StringTraits<TChar>::Literal(" ]", L" ]");
  return os;
}

// queues
template <typename TChar, typename TTraits, typename TItem>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, std::queue<TItem> queue) {
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

// pairs
template <typename TChar, typename TTraits, typename TFirst, typename TSecond>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, std::pair<TFirst, TSecond> value) {
  os << StringTraits<TChar>::Literal("(", L"(");
  PrettyPrint(os, value.first);
  os << StringTraits<TChar>::Literal(", ", L", ");
  PrettyPrint(os, value.second);
  os << StringTraits<TChar>::Literal(")", L")");
  return os;
}

// Catch-all for everything else.
template <typename TChar, typename TTraits, typename TItem, typename std::enable_if<!is_container<TItem>::value>::type*>
auto& PrettyPrint(std::basic_ostream<TChar, TTraits>& os, TItem item) {
  os << item;
  return os;
}

// Prints args with separator between them. const char* separator.
template <typename TChar, typename TTraits, typename... TArgs>
auto& PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits>& os, const TChar* separator, TArgs&&... args) {
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
auto& PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits>& os,
                               std::basic_string<TChar, TTraits> separator,
                               TArgs&&... args) {
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
auto& PrettyPrintWithSeparator(std::basic_ostream<TChar, TTraits>& os,
                               std::basic_string_view<TChar, TTraits> separator,
                               TArgs&&... args) {
  if (sizeof...(TArgs) == 0) {
    os << StringTraits<TChar>::Literal("", L"");
    return os;
  }
  size_t n = 0;
  ((PrettyPrint(os, args) << (++n != sizeof...(TArgs) ? separator : StringTraits<TChar>::Literal("", L""))), ...);
  return os;
}

}  // End namespace CPPUtils

/** @}*/
#endif  // End !defined CPP_UTILS_pretty_print_h__
