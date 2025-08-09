find_package(GTest REQUIRED)

add_executable(spectrum-analyzer-tests
        FFTCalculatorTests.cpp
        DataCalculatorTests.cpp
        IndexSelectorTests.cpp
	StatsTests.cpp
	ConfigReaderTests.cpp
        SamplesCollectorTests.cpp
        WindowTests.cpp
        AudioSpectrumAnalyzerTests.cpp
        helpers/TextInsideGpuMock.cpp
        helpers/OpenGlMock.cpp
        helpers/TestHelpers.cpp
        )

target_include_directories(spectrum-analyzer-tests
  PRIVATE
    ${CMAKE_INCLUDE_CURRENT_DIR}
    "../${CMAKE_INCLUDE_CURRENT_DIR}"
  )

target_link_libraries(spectrum-analyzer-tests PRIVATE
    spectrum-analyzer-core
    GTest::GTest
    GTest::gmock
    GTest::gmock_main
)


add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/testConfig.py
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/testConfig.py
                                          ${CMAKE_CURRENT_BINARY_DIR}/testConfig.py
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/testConfig.py
  )
add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/testAudioConfig.py
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/testAudioConfig.py
                                          ${CMAKE_CURRENT_BINARY_DIR}/testAudioConfig.py
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/testAudioConfig.py
  )
target_sources(spectrum-analyzer-tests
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/testConfig.py
    ${CMAKE_CURRENT_BINARY_DIR}/testAudioConfig.py
  )
