
pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)
pkg_search_module(PORTAUDIO REQUIRED IMPORTED_TARGET portaudio-2.0)

add_library(spectrum-analyzer-core STATIC
    PythonCodeRunner.cpp
    SamplesCollector.cpp
    ConfigReader.cpp
    FFTCalculator.cpp
    WindowBase.cpp
    Window.cpp
    DataSelector.cpp
    DataCalculator.cpp
    Stats.cpp
    AudioSamplesCollector.cpp
    AudioSpectrumAnalyzer.cpp
    Helpers.cpp
    RectangleHighligther.cpp
    DynamicMaxHolder.cpp
    gpu/ElementInsideGpu.cpp
    gpu/LineInsideGpu.cpp
    gpu/RectangleInsideGpu.cpp
    gpu/TextInsideGpu.cpp
    gpu/FigureGeometryCalculator.cpp
    gpu/Gpu.cpp
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
    PkgConfig::PORTAUDIO
  )

