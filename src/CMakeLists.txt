# Define the ansi_escapes library
add_library(ansi_escapes ansi_escapes.cpp)
target_include_directories(ansi_escapes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the console_logger library
add_library(console_logger console_logger.cpp)
target_include_directories(console_logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(console_logger PRIVATE logger)

# Define the logger library
add_library(logger logger.cpp)
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the pretty_print library
add_library(pretty_print pretty_print.cpp)
target_include_directories(pretty_print PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the windows_logger library only for Windows
if(WIN32)
    add_library(windows_logger windows_logger.cpp)
    target_include_directories(windows_logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(windows_logger PRIVATE logger)
    set_target_properties(windows_logger PROPERTIES COMPILE_DEFINITIONS "_WIN32")
endif()

# Define the tinytest library
add_library(tinytest tinytest.cpp)
target_include_directories(tinytest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tinytest PRIVATE pretty_print) 