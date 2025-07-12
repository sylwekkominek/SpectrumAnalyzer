
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(spectrum-analyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(OpenGL 4.5 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)

add_subdirectory(glad/)
add_subdirectory(core/)

option(ENABLE_TESTS "Enable tests" OFF)

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


    add_custom_command(
      OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/audioConfig.py
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/audioConfig.py
                                              ${CMAKE_CURRENT_BINARY_DIR}/audioConfig.py
      DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/audioConfig.py
      )

    add_custom_command(
      OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/config.py
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/config.py
                                              ${CMAKE_CURRENT_BINARY_DIR}/config.py
      DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/config.py
      )

    target_sources(spectrum-analyzer
      PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/audioConfig.py
        ${CMAKE_CURRENT_BINARY_DIR}/config.py
      )

endif()


