# CMakeLists.txt for cppSwarmLib project
cmake_minimum_required(VERSION 3.20)  # Requires CMake 3.20+ for proper C++20 support
project(cppSwarmLib LANGUAGES CXX)    # Define project name and language

# Set C++20 standard with required support
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)         # Disable compiler-specific extensions

# Create interface library (header-only)
add_library(cppSwarmLib INTERFACE)

# Add public include directories for consumers
target_include_directories(cppSwarmLib
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>   # For build tree
        $<INSTALL_INTERFACE:include>                     # For install tree
)

# Install headers preserving directory structure
install(
    DIRECTORY 
        ${CMAKE_CURRENT_SOURCE_DIR}/Tasks/
        ${CMAKE_CURRENT_SOURCE_DIR}/UnitComponent/
        ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION 
        include/cppSwarmLib
    FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "CMakeLists.txt" EXCLUDE  # Exclude CMakeLists from installation
)

# Install the library target
install(TARGETS cppSwarmLib
    EXPORT cppSwarmLibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)