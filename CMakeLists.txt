
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(spectrum-analyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Enable tests" OFF)
option(NO_PYTHON "Disable python" OFF)
option(ENABLE_LOOPBACK "Enable loopback" OFF)

find_package(OpenGL 4.5 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)

if(NOT NO_PYTHON OR ENABLE_TESTS)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
endif()


add_subdirectory(glad/)
add_subdirectory(core/)


if(ENABLE_TESTS)
    message(STATUS "Running tests")
    add_subdirectory(tests/)
else()
    message(STATUS "Building app")

    add_executable(spectrum-analyzer main.cpp)

    target_include_directories(spectrum-analyzer
      PRIVATE
        ${CMAKE_INCLUDE_CURRENT_DIR}
      )

    target_link_libraries(spectrum-analyzer PRIVATE
        spectrum-analyzer-core)

    if(NOT NO_PYTHON)
        add_custom_command(
          OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/audioConfig.py
          COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/audioConfig.py
                                                  ${CMAKE_CURRENT_BINARY_DIR}/audioConfig.py
          DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/audioConfig.py
          )

        target_sources(spectrum-analyzer
          PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/audioConfig.py
          )
    endif()

endif()


