########################################################################################################################
#                                                                                                                      #
# @file BUILD                                                                                                          #
#                                                                                                                      #
# @brief Defines build rules including tests for the top level utilities.                                              #
# @copyright Copyright (C) 2023 by Tom Hicks <headhunter3@gmail.com                                                    #
#                                                                                                                      #
# Licensed under the MIT license. See below for details.                                                               #
#                                                                                                                      #
#  MIT License                                                                                                         #
#                                                                                                                      #
# Copyright (c) 2023 Tom Hicks <headhunter3@gmail.com>                                                                 #
#                                                                                                                      #
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated         #
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation the  #
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to      #
# permit persons to whom the Software is furnished to do so, subject to the following conditions:                      #
#                                                                                                                      #
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the #
# Software.                                                                                                            #
#                                                                                                                      #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE #
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS   #
# OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR  #
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.     #
#                                                                                                                      #
########################################################################################################################
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "ansi_escapes",
    srcs = ["ansi_escapes.cpp"],
    hdrs = ["ansi_escapes.h"],
    includes = ["ansi_escapes.h"],
)

cc_test(
    name = "ansi_escapes_test",
    srcs = ["ansi_escapes_test.cpp"],
    deps = [
        ":ansi_escapes",
        "@tinytest",
    ],
)

cc_library(
    name = "console_logger",
    srcs = ["console_logger.cpp"],
    hdrs = ["console_logger.h"],
    deps = [":logger"],
)

cc_test(
    name = "console_logger_test",
    srcs = ["console_logger_test.cpp"],
    deps = [
        ":console_logger",
        "@tinytest",
    ],
)

cc_library(
    name = "logger",
    srcs = ["logger.cpp"],
    hdrs = ["logger.h"],
)

cc_test(
    name = "logger_test",
    srcs = ["logger_test.cpp"],
    deps = [
        ":logger",
        "@tinytest",
    ],
)

cc_library(
    name = "pretty_print",
    srcs = ["pretty_print.cpp"],
    hdrs = ["pretty_print.h"],
    includes = ["pretty_print.h"],
)

cc_test(
    name = "pretty_print_test",
    srcs = ["pretty_print_test.cpp"],
    deps = [
        ":pretty_print",
        "@tinytest",
    ],
)

cc_library(
    name = "windows_logger",
    srcs = ["windows_logger.cpp"],
    hdrs = ["windows_logger.h"],
    target_compatible_with = ["@platforms//os:windows"],
    deps = [":logger"],
)

cc_test(
    name = "windows_logger_test",
    srcs = ["windows_logger_test.cpp"],
    target_compatible_with = ["@platforms//os:windows"],
    deps = [
        ":windows_logger",
        "@tinytest",
    ],
)
