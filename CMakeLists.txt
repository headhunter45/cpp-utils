cmake_minimum_required(VERSION 3.10)

project(CPPUtils VERSION 1.0 LANGUAGES CXX)
enable_testing()

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories for each component
add_subdirectory(src)
add_subdirectory(tests)

# Include FetchContent module
include(FetchContent)

# Add Google Test as an external dependency
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest) 