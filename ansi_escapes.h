#ifndef CPP_Utils__ansi_escapes_h__
#define CPP_Utils__ansi_escapes_h__
/***************************************************************************************
 * @file ansi_escapes.h                                                                *
 *                                                                                     *
 * @brief Defines constants and functions for working with screen colors.              *
 * @copyright                                                                          *
 * Copyright 2023 Tom Hicks                                                            *
 * Licensed under the MIT license see the LICENSE file for details.                    *
 ***************************************************************************************/
#include <cstdint>
#include <ostream>
#include <vector>

/** \addtogroup Ansi Escape Sequences
 * @{
 */
namespace CPPUtils {
constexpr uint8_t GetRedComponent(uint32_t color) {
  return (color & 0x00FF0000) >> 16;
}

constexpr uint8_t GetGreenComponent(uint32_t color) {
  return (color & 0x0000FF00) >> 8;
}

constexpr uint8_t GetBlueComponent(uint32_t color) {
  return (color & 0x000000FF);
}

template <typename TChar, typename TTraits>
auto& Escape(std::basic_ostream<TChar, TTraits>& os, const std::basic_string<TChar, TTraits>& escape_code) {
  return os << "\033[" << escape_code << "m";
}

template <typename TChar, typename TTraits>
auto& Escape(std::basic_ostream<TChar, TTraits>& os, const std::basic_string_view<TChar, TTraits>& escape_code) {
  return os << "\033[" << escape_code << "m";
}

template <typename TChar, typename TTraits>
auto& Escape(std::basic_ostream<TChar, TTraits>& os, const TChar* escape_code) {
  return os << "\033[" << escape_code << "m";
}

template <typename TChar, typename TTraits>
auto& ForegroundColor8Bit(std::basic_ostream<TChar, TTraits>& os, uint8_t color) {
  return Escape(os, "38;5;" + std::to_string(color));
}

template <typename TChar, typename TTraits>
auto& BackgroundColor8Bit(std::basic_ostream<TChar, TTraits>& os, uint8_t color) {
  return Escape(os, "48;5;" + std::to_string(color));
}

template <typename TChar, typename TTraits>
auto& ForegroundTrueColor(std::basic_ostream<TChar, TTraits>& os, uint8_t red, uint8_t green, uint8_t blue) {
  return os << "\033["
            << "38;2;" << (uint16_t)red << ";" << (uint16_t)green << ";" << (uint16_t)blue << "m";
}

template <typename TChar, typename TTraits>
auto& ForegroundTrueColor(std::basic_ostream<TChar, TTraits>& os, uint32_t color) {
  return ForegroundTrueColor(os, GetRedComponent(color), GetGreenComponent(color), GetBlueComponent(color));
}

template <typename TChar, typename TTraits>
auto& BackgroundTrueColor(std::basic_ostream<TChar, TTraits>& os, uint8_t red, uint8_t green, uint8_t blue) {
  return os << "\033["
            << "48;2;" << (uint16_t)red << ";" << (uint16_t)green << ";" << (uint16_t)blue << "m";
}

template <typename TChar, typename TTraits>
auto& BackgroundTrueColor(std::basic_ostream<TChar, TTraits>& os, uint32_t color) {
  return BackgroundTrueColor(os, GetRedComponent(color), GetGreenComponent(color), GetBlueComponent(color));
}

template <typename TChar, typename TTraits>
auto& Reset(std::basic_ostream<TChar, TTraits>& os) {
  return os << "\033["
            << "m";
}
}  // End namespace CPPUtils

/** @}*/
#endif  // End !defined CPP_Utils__ansi_escapes_h__
