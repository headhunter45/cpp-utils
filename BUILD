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

