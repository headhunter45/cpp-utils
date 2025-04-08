# Enable testing
enable_testing()

# Define the ansi_escapes_test executable
add_executable(ansi_escapes_test ansi_escapes_test.cpp)
target_link_libraries(ansi_escapes_test PRIVATE ansi_escapes tinytest)
add_test(NAME ansi_escapes_test COMMAND ansi_escapes_test)

# Define the console_logger_test executable
add_executable(console_logger_test console_logger_test.cpp)
target_link_libraries(console_logger_test PRIVATE console_logger tinytest)
add_test(NAME console_logger_test COMMAND console_logger_test)

# Define the logger_test executable
add_executable(logger_test logger_test.cpp)
target_link_libraries(logger_test PRIVATE logger tinytest)
add_test(NAME logger_test COMMAND logger_test)

# Define the pretty_print_test executable
add_executable(pretty_print_test pretty_print_test.cpp)
target_link_libraries(pretty_print_test PRIVATE pretty_print tinytest)
add_test(NAME pretty_print_test COMMAND pretty_print_test)

# Define the windows_logger_test executable only for Windows
if(WIN32)
    add_executable(windows_logger_test windows_logger_test.cpp)
    target_link_libraries(windows_logger_test PRIVATE windows_logger tinytest)
    add_test(NAME windows_logger_test COMMAND windows_logger_test)
endif()

# Define the tinytest_test executable
add_executable(tinytest_test tinytest_test.cpp)
target_link_libraries(tinytest_test PRIVATE tinytest gtest_main gtest gmock)
add_test(NAME tinytest_test COMMAND tinytest_test) 