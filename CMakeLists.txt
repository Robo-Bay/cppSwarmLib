# CMakeLists.txt for cppSwarmLib project
cmake_minimum_required(VERSION 3.20)  # Requires CMake 3.20+ for proper C++20 support
project(cppSwarmLib LANGUAGES CXX)    # Define project name and language

# Set C++20 standard with required support
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)         # Disable compiler-specific extensions

add_compile_options( # Add options for compile
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

# Create interface library (header-only)
add_library(cppSwarmLib INTERFACE)

# Add public include directories for consumers
target_include_directories(cppSwarmLib
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>   # For build tree
        $<INSTALL_INTERFACE:include>                     # For install tree
)

# Опция для включения тестов
option(BUILD_TESTS "Build test executables" OFF)

if(BUILD_TESTS)
    # Поиск Boost с компонентом Unit Test Framework
    find_package(Boost 1.71 REQUIRED COMPONENTS unit_test_framework)
    
    # Проверка успешного поиска Boost
    if(Boost_FOUND)
        # Включение системы тестирования CMake
        enable_testing()
        
        # Найти все файлы .cpp в директории tests
        file(GLOB TEST_SOURCES "src/Tests/*.cpp")
        list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Tests/test_main.cpp") # Если уже есть
        list(APPEND TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Tests/test_main.cpp")
        
        # Проверить, что найдены тестовые файлы
        if(NOT TEST_SOURCES)
            message(WARNING "No test files found in the 'tests' directory!")
        else()
            # Добавление исполняемого файла для тестов
            add_executable(cppSwarmLibTests
                ${TEST_SOURCES}  # Все найденные .cpp файлы
            )
            
            # Настройка линковки тестов
            target_link_libraries(cppSwarmLibTests
                PRIVATE
                    cppSwarmLib
                    Boost::unit_test_framework
            )
            
            # Добавление теста в CTest
            add_test(
                NAME cppSwarmLibUnitTests
                COMMAND cppSwarmLibTests
            )
            
            # Дополнительные настройки для тестов
            target_compile_definitions(cppSwarmLibTests PRIVATE BOOST_TEST_DYN_LINK)
            # target_compile_definitions(cppSwarmLibTests PRIVATE BOOST_TEST_MAIN )
            
            # Отключение -Werror для тестов (опционально)
            target_compile_options(cppSwarmLibTests PRIVATE -Wno-error)
        endif()
    endif()
endif()


# Install headers preserving directory structure
install(
    DIRECTORY 
        ${CMAKE_CURRENT_SOURCE_DIR}/Tasks/
        ${CMAKE_CURRENT_SOURCE_DIR}/UnitComponent/
        ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION 
        include/cppSwarmLib
    FILES_MATCHING
        PATTERN "src/*.hpp"
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