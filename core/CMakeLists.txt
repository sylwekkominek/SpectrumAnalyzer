
pkg_search_module(PORTAUDIO REQUIRED IMPORTED_TARGET portaudio-2.0)
pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)


add_library(device-selection-lib STATIC)
add_library(samples-collector-lib STATIC)

if(ENABLE_LOOPBACK AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Linux loopback enabled")
    target_sources(device-selection-lib PRIVATE
            dataSource/LoopbackAudioDataSourceLinux.cpp)

elseif(ENABLE_LOOPBACK AND WIN32)
    message(STATUS "Windows loopback enabled")
    target_sources(device-selection-lib PRIVATE
            dataSource/LoopbackAudioDataSourceWindows.cpp)
else()
    message(STATUS "Microphone as audio data source selected - loopback disabled")
    target_sources(device-selection-lib PRIVATE
            dataSource/MicrophoneAudioDataSource.cpp)
endif()

target_link_libraries(device-selection-lib
  PUBLIC
    PkgConfig::PORTAUDIO
  )


if(NO_PYTHON AND NOT ENABLE_TESTS)
    message(STATUS "Building without Python")
    target_sources(samples-collector-lib PRIVATE
        dataSource/SamplesCollectorWithoutPython.cpp
        dataSource/AudioDataSource.cpp
    )
else()
    message(STATUS "Building with Python")
    target_sources(samples-collector-lib PRIVATE
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
      )
endif()


target_link_libraries(samples-collector-lib
  PUBLIC
    PkgConfig::PORTAUDIO
    device-selection-lib
  )


add_library(spectrum-analyzer-config STATIC
    config/AdvancedColorSettings.cpp
    config/AlphaFactor.cpp
    config/BackgroundColorSettings.cpp
    config/ColorOfDynamicMaxHoldLine.cpp
    config/ColorOfDynamicMaxHoldSecondaryLine.cpp
    config/ColorOfLine.cpp
    config/ColorOfStaticLines.cpp
    config/ColorOfStaticText.cpp
    config/ColorsOfDynamicMaxHoldRectangle.cpp
    config/ColorsOfDynamicMaxHoldSecondaryRectangle.cpp
    config/ColorsOfRectangle.cpp
    config/DefaultFullscreenState.cpp
    config/DesiredFrameRate.cpp
    config/DynamicMaxHoldAccelerationStateOfFalling.cpp
    config/DynamicMaxHoldRectangleHeightInPercentOfScreenSize.cpp
    config/DynamicMaxHoldSecondarySpeedOfFalling.cpp
    config/DynamicMaxHoldSecondaryVisibilityState.cpp
    config/DynamicMaxHoldSpeedOfFalling.cpp
    config/DynamicMaxHoldVisibilityState.cpp
    config/Frequencies.cpp
    config/FrequencyTextPositions.cpp
    config/GapWidthInRelationToRectangleWidth.cpp
    config/HorizontalLinePositions.cpp
    config/LinesVisibilityState.cpp
    config/MaximizedWindowSize.cpp
    config/MaxQueueSize.cpp
    config/NormalWindowSize.cpp
    config/NumberOfRectangles.cpp
    config/NumberOfSamples.cpp
    config/NumberOfSignalsForAveraging.cpp
    config/NumberOfSignalsForMaxHold.cpp
    config/OffsetFactor.cpp
    config/PythonDataSourceEnabled.cpp
    config/RectanglesVisibilityState.cpp
    config/SamplingRate.cpp
    config/ScalingFactor.cpp
    config/SignalWindow.cpp
    config/VerticalLinePositions.cpp
    config/WindowTitle.cpp
)

target_include_directories(spectrum-analyzer-config
  PUBLIC
    "${CMAKE_INCLUDE_CURRENT_DIR}"
    "${CMAKE_INCLUDE_CURRENT_DIR}/config"
  )



add_library(spectrum-analyzer-core STATIC

    ConfigFileReader.cpp
    Config.cpp
    ConfigReader.cpp
    FFTCalculator.cpp
    WindowBase.cpp
    Window.cpp
    FrequenciesInfo.cpp
    DataCalculator.cpp
    Stats.cpp
    AudioSpectrumAnalyzer.cpp
    Helpers.cpp
    RectangleHighligther.cpp
    DynamicMaxHolder.cpp
    PowerAverager.cpp
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
    spectrum-analyzer-config
  )







