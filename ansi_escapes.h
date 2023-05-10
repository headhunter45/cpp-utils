/**********************************************************************************************************************
 *
 * @file ansi_escapes.h
 *
 * @brief Defines constants and functions for working with screen colors.
 *
 * @copyright Copyright (C) 2023 by Tom Hicks <headhunter3@gmail.com>
 *
 * Licensed under the MIT license see below for details.
 *
 **********************************************************************************************************************/
/*
 *
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
#ifndef CPPUtils__ansi_escapes_h__
#define CPPUtils__ansi_escapes_h__
#include "pretty_print.h"
#include <cstdint>
#include <ostream>
#include <vector>

namespace CPPUtils {
/// @addtogroup ansi_escape_sequences Ansi Escape Sequences
/// @{

/// @brief Gets the red component of a 32bit ARGB color value.
/// @param color The color to split.
/// @return The red component as an 8-bit unsigned int.
constexpr uint8_t GetRedComponent(uint32_t color) {
  return (color & 0x00FF0000) >> 16;
}

/// @brief Gets the green component of a 32bit ARGB color value.
/// @param color The color to split.
/// @return The green component as an 8-bit unsigned int.
constexpr uint8_t GetGreenComponent(uint32_t color) {
  return (color & 0x0000FF00) >> 8;
}

/// @brief Gets the blue component of a 32bit ARGB color value.
/// @param color The color to split.
/// @return The blue component as an 8-bit unsigned int.
constexpr uint8_t GetBlueComponent(uint32_t color) {
  return (color & 0x000000FF);
}

/// @brief Gets the alpha component of a 32bit ARGB color value.
///
/// This function is included for completeness. You cannot set a transparent color via these ansi escape sequences.
/// @param color The color to split.
/// @return The alpha component as an 8-bit unsigned int.
constexpr uint8_t GetAlphaComponent(uint32_t color) {
  return (color & 0xFF000000);
}

/// @brief Writes the escape_code string provided as an ansi escape sequence.
///
/// For example to set the foreground to 24bit red you would pass "38;2;255;0;0" as escape_code and this would print
/// "\033" + "38;2;255;0;0" + "m".
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @param escape_code The ansi sequence to escape.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &Escape(std::basic_ostream<TChar, TTraits> &os, const std::basic_string<TChar, TTraits> &escape_code) {
  return os << StringTraits<TChar>::Literal("\033[", L"\033[") << escape_code
            << StringTraits<TChar>::Literal("m", L"m");
}

/// @brief Writes the escape_code string provided as an ansi escape sequence.
///
/// For example to set the foreground to 24bit red you would pass "38;2;255;0;0" as escape_code and this would print
/// "\033" + "38;2;255;0;0" + "m".
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @param escape_code The ansi sequence to escape.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &Escape(std::basic_ostream<TChar, TTraits> &os, const std::basic_string_view<TChar, TTraits> &escape_code) {
  return os << StringTraits<TChar>::Literal("\033[", L"\033[") << escape_code
            << StringTraits<TChar>::Literal("m", L"m");
}

/// @brief Writes the escape_code string provided as an ansi escape sequence.
///
/// For example to set the foreground to 24bit red you would pass "38;2;255;0;0" as escape_code and this would print
/// "\033" + "38;2;255;0;0" + "m".
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @param escape_code The ansi sequence to escape.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &Escape(std::basic_ostream<TChar, TTraits> &os, const TChar *escape_code) {
  return os << StringTraits<TChar>::Literal("\033[", L"\033[") << escape_code
            << StringTraits<TChar>::Literal("m", L"m");
}

/// @brief Writes an ansi escape sequence that sets the foreground color to one of the 8-bit pallete colors.
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @param color The color value to set.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &ForegroundColor8Bit(std::basic_ostream<TChar, TTraits> &os, uint8_t color) {
  return Escape(os,
                StringTraits<TChar>::Literal("38;5;", L"38;5;")
                    + StringTraits<TChar>::Literal(std::to_string(color), std::to_wstring(color)));
}

/// @brief Writes an ansi escape sequence that sets the background color to one of the 8-bit pallete colors.
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @param color The color value to set.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &BackgroundColor8Bit(std::basic_ostream<TChar, TTraits> &os, uint8_t color) {
  return Escape(os,
                StringTraits<TChar>::Literal("48;5;", L"48;5;")
                    + StringTraits<TChar>::Literal(std::to_string(color), std::to_wstring(color)));
}

/// @brief Writes an ansi escape sequence that sets the foreground color to one of the 24-bit pallete colors.
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @param red The red value to set.
/// @param green The green value to set.
/// @param blue The blue value to set.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &ForegroundTrueColor(std::basic_ostream<TChar, TTraits> &os, uint8_t red, uint8_t green, uint8_t blue) {
  std::basic_ostringstream<TChar> str;
  str << StringTraits<TChar>::Literal("38;2;", L"38;2;") << (uint16_t)red << StringTraits<TChar>::Literal(";", L";")
      << (uint16_t)green << StringTraits<TChar>::Literal(";", L";") << (uint16_t)blue;
  return Escape(os, str.str());
}

/// @brief Writes an ansi escape sequence that sets the foreground color to one of the 24-bit pallete colors.
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @param color The encoded ARGB value to set.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &ForegroundTrueColor(std::basic_ostream<TChar, TTraits> &os, uint32_t color) {
  return ForegroundTrueColor(os, GetRedComponent(color), GetGreenComponent(color), GetBlueComponent(color));
}

/// @brief Writes an ansi escape sequence that sets the background color to one of the 24-bit pallete colors.
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @param red The red value to set.
/// @param green The green value to set.
/// @param blue The blue value to set.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &BackgroundTrueColor(std::basic_ostream<TChar, TTraits> &os, uint8_t red, uint8_t green, uint8_t blue) {
  std::basic_ostringstream<TChar> str;
  str << StringTraits<TChar>::Literal("48;2;", L"48;2;") << (uint16_t)red << StringTraits<TChar>::Literal(";", L";")
      << (uint16_t)green << StringTraits<TChar>::Literal(";", L";") << (uint16_t)blue;
  return Escape(os, str.str());
}

/// @brief Writes an ansi escape sequence that sets the background color to one of the 24-bit pallete colors.
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @param color The encoded ARGB value to set.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits>
auto &BackgroundTrueColor(std::basic_ostream<TChar, TTraits> &os, uint32_t color) {
  return BackgroundTrueColor(os, GetRedComponent(color), GetGreenComponent(color), GetBlueComponent(color));
}

/// @brief Writes an empty escape sequence to reset the output stream.
/// @tparam TChar The type of character in the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @tparam TTraits The char_traits of the std::basic_ostream you are writing to.
///
/// Usually this will be automatically detected from the stream and string you pass in so you shouldn't need to specify
/// it.
/// @param os The output stream to write to.
/// @return The output stream for chaining.
template <typename TChar, typename TTraits> auto &Reset(std::basic_ostream<TChar, TTraits> &os) {
  return Escape(os, "");
}

/// @}
} // End namespace CPPUtils

#endif // End !defined CPPUtils__ansi_escapes_h__
