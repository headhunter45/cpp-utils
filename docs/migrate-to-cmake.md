# Migration Plan: Bazel to CMake

## 1. Understand the Current Build Configuration
### Review Bazel Files
- Identify all Bazel build files (`BUILD`, `WORKSPACE`, etc.) and understand their structure and dependencies.
- **Specific Files**:
  - `BUILD`
  - `WORKSPACE`

### Identify Targets
- List all the targets, libraries, and executables defined in the Bazel files.

#### Libraries
- `ansi_escapes`
- `console_logger`
- `logger`
- `pretty_print`
- `windows_logger`

#### Tests
- `ansi_escapes_test`
- `console_logger_test`
- `logger_test`
- `pretty_print_test`
- `windows_logger_test`

### Dependencies
- Note all external dependencies, including JTest, and how they are currently managed.

#### External Dependencies
##### Hedron's Compile Commands Extractor for Bazel
- **Name**: `hedron_compile_commands`
- **URL**: [Hedron Compile Commands Extractor](https://github.com/hedronvision/bazel-compile-commands-extractor)
- **SHA256**: `99bc3106eb6ce5ffab3c31de8501d4d628de5f1acd74b8b563a876bd39a2e32f`

###### How to Generate `compile_commands.json` with CMake
- **Configure CMake to Output `compile_commands.json`**:
  - When running CMake, specify the `CMAKE_EXPORT_COMPILE_COMMANDS` option to generate the `compile_commands.json` file.
  - Add the following line to your `CMakeLists.txt` or pass it as a command-line argument:
  
  ```bash
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
  ```

- **Build the Project**:
  - After configuring CMake with the above option, build the project. The `compile_commands.json` file will be generated in the build directory.

##### TinyTest
- **Name**: `TinyTest`
- **URL**: [TinyTest](https://github.com/headhunter45/TinyTest)
- **SHA256**: `d6729abbec6ac167635be7463d8c37ca54b08e506e61553236b50c5ad30e8736`

**Note**: TinyTest is now a git submodule under `external/TinyTest`. We will replace the external dependency with a dependency on this folder and migrate TinyTest to use CMake within that folder.

## 2. Set Up CMake Structure
- **Create CMakeLists.txt**: For each Bazel `BUILD` file, create a corresponding `CMakeLists.txt` file.
- **Define Project**: Use `project()` to define the project name and version in the root `CMakeLists.txt`.
- **Set CMake Version**: Specify the minimum required CMake version using `cmake_minimum_required()`.

## 3. Translate Build Rules
- **Targets**: Convert Bazel targets to CMake targets using `add_executable()` and `add_library()`.
- **Include Directories**: Use `include_directories()` to specify include paths.
- **Link Libraries**: Use `target_link_libraries()` to link against necessary libraries.

## 4. Handle Dependencies
- **External Libraries**: Use `find_package()` or `FetchContent` to manage external dependencies.
- **JTest Migration**: Since JTest will also be migrated to CMake, ensure it is included as a dependency using CMake's dependency management.

## 5. Configure Build Options
- **Compiler Flags**: Translate any Bazel-specific compiler flags to CMake using `set()` and `add_compile_options()`.
- **Build Types**: Define build types (e.g., Debug, Release) using `set(CMAKE_BUILD_TYPE)`.

## 6. Testing and Validation
- **Build the Project**: Use CMake to configure and build the project, ensuring all targets are correctly built.
- **Run Tests**: If there are tests, ensure they are correctly configured and run using CMake's testing support (`enable_testing()` and `add_test()`).

## 7. Documentation and Cleanup
- **Document Changes**: Update any project documentation to reflect the new build system.
- **Remove Bazel Files**: Once the migration is complete and verified, remove Bazel-related files.

## 8. Continuous Integration
- **Update CI/CD**: Modify any CI/CD pipelines to use CMake instead of Bazel for building and testing.

## 9. Iterate and Refine
- **Feedback Loop**: Gather feedback from developers and refine the CMake setup as needed. 