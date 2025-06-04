
pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)

add_library(spectrum-analyzer-core STATIC
    PythonCodeRunner.cpp
    SamplesCollector.cpp
    ConfigReader.cpp
    FFTCalculator.cpp
    Window.cpp
    IndexSelector.cpp
    DataCalculator.cpp
    Stats.cpp
    AudioSpectrumAnalyzer.cpp
    Helpers.cpp
)

target_include_directories(spectrum-analyzer-core
  PUBLIC
    "../${CMAKE_INCLUDE_CURRENT_DIR}"
    ${PYTHON_INCLUDE_DIRS}
  )

target_link_libraries(spectrum-analyzer-core
  PUBLIC
    ${PYTHON_LIBRARIES}
    OpenGL::GL
    glfw
    glad
    PkgConfig::FFTW
  )

