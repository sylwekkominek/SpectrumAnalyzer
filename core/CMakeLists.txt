
pkg_search_module(PORTAUDIO REQUIRED IMPORTED_TARGET portaudio-2.0)
pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)

if(NO_PYTHON AND NOT ENABLE_TESTS)
    message(STATUS "Building without Python")
    add_library(samples-collector-lib STATIC
        dataSource/SamplesCollectorWithoutPython.cpp
        dataSource/AudioDataSource.cpp
    )

    target_link_libraries(samples-collector-lib
      PUBLIC
        PkgConfig::PORTAUDIO
      )

else()
    message(STATUS "Building with Python")
    add_library(samples-collector-lib STATIC
        dataSource/PythonCodeRunner.cpp
        dataSource/PythonDataSource.cpp
        dataSource/SamplesCollectorWithPython.cpp
        dataSource/AudioDataSource.cpp
    )

    target_include_directories(samples-collector-lib
      PUBLIC
        ${Python3_INCLUDE_DIRS}
      )

    target_link_libraries(samples-collector-lib
      PUBLIC
        ${Python3_LIBRARIES}
        PkgConfig::PORTAUDIO
      )
endif()


add_library(spectrum-analyzer-core STATIC

    ConfigFileReader.cpp
    Config.cpp
    ConfigReader.cpp
    FFTCalculator.cpp
    WindowBase.cpp
    Window.cpp
    DataSelector.cpp
    DataCalculator.cpp
    Stats.cpp
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
  )


target_link_libraries(spectrum-analyzer-core
  PUBLIC
    OpenGL::GL
    glfw
    glad
    PkgConfig::FFTW
    samples-collector-lib
  )







