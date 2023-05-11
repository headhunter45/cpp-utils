########################################################################################################################
#                                                                                                                      #
# @file WORKSPACE                                                                                                      #
#                                                                                                                      #
# @brief Defines external modules available to all BUILD files in this workspace. This also marks the root folder of   #
# the workspace                                                                                                        #
# @copyright Copyright (C) 2023 by Tom Hicks <headhunter3@gmail.com                                                    #
#                                                                                                                      #
# Licensed under the MIT license. See below for details.                                                               #
#                                                                                                                      #
########################################################################################################################
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
workspace(name = "CPPUtils")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
# To update config run `bazel run @hedron_compile_commands//:refresh_all`
http_archive(
    name = "hedron_compile_commands",
    sha256 = "99bc3106eb6ce5ffab3c31de8501d4d628de5f1acd74b8b563a876bd39a2e32f",
    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    strip_prefix = "bazel-compile-commands-extractor-b33a4b05c2287372c8e932c55ff4d3a37e6761ed",
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/b33a4b05c2287372c8e932c55ff4d3a37e6761ed.tar.gz",
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()

http_archive(
    name = "TinyTest",
    sha256 = "d6729abbec6ac167635be7463d8c37ca54b08e506e61553236b50c5ad30e8736",
    strip_prefix = "TinyTest-a4109d2f494fd3fd3cc47069239fd3c79f728d5a",
    urls = ["https://github.com/headhunter45/TinyTest/archive/a4109d2f494fd3fd3cc47069239fd3c79f728d5a.zip"],
)
