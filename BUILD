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
