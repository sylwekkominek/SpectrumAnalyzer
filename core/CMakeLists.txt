
pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)

add_library(spectrum-analyzer-core STATIC
    PythonCodeRunner.cpp
    SamplesCollector.cpp
    ConfigReader.cpp
    FFTCalculator.cpp
    WindowBase.cpp
    Window.cpp
    IndexSelector.cpp
    DataCalculator.cpp
    Stats.cpp
    AudioSpectrumAnalyzer.cpp
    Helpers.cpp
    gpu/ElementInsideGpu.cpp
    gpu/LineInsideGpu.cpp
    gpu/RectangleInsideGpu.cpp
    gpu/TextInsideGpu.cpp
)

target_include_directories(spectrum-analyzer-core
  PUBLIC
    "${CMAKE_INCLUDE_CURRENT_DIR}"
    "${CMAKE_INCLUDE_CURRENT_DIR}/gpu"
    ${Python3_INCLUDE_DIRS}
  )

target_link_libraries(spectrum-analyzer-core
  PUBLIC
    ${Python3_LIBRARIES}
    OpenGL::GL
    glfw
    glad
    PkgConfig::FFTW
  )

