cmake_minimum_required(VERSION 3.13)
project(cppSwarmLib VERSION 1.0 LANGUAGES CXX)

# Настройка компилятора
add_compile_options(
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

# Отключение расширений компилятора
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()


# Добавление исходных файлов
set(SOURCES
    src/Params.hpp
    src/SwarmUnit.hpp
    src/Swarm.hpp
    src/Tasks/ITask.hpp
    src/UnitComponent/IUnitComponent.hpp
    src/UnitComponent/ITaskManagerC.hpp
    src/UnitComponent/IExecutorC.hpp
    src/UnitComponent/ICommunicationC.hpp
)

# Создание библиотеки
add_library(cppSwarmLib STATIC ${SOURCES})
target_link_libraries(cppSwarmLib PRIVATE src/Swarm.hpp)

# Установка директории для заголовочных файлов
target_include_directories(cppSwarmLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Установка стандарта C++
set_target_properties(cppSwarmLib PROPERTIES
    CXX_STANDARD 20 
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)