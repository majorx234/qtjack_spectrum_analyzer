cmake_minimum_required(VERSION 3.16)
project(qtjack_spectrum_analyzer
  DESCRIPTION "analyzer for Fourier spectrum of audio signals"
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Threads)
find_package(qtjack)
find_package(PkgConfig)
pkg_check_modules(FFTW3 REQUIRED fftw3)

set(FFTW_TEST_SOURCES
  src/test/fftw_test.cpp
)

set(SPECTRUM_ANALYZER_SOURCES
  src/spectrum_analyzer.cpp
)

set(SPECTRUM_ANALYZER_HEADERS
  include/spectrum_analyzer.hpp
)

set(SPECTRUM_ANALYZER_TEST_SOURCES
  src/test/spectrum_analyzer_test.cpp
)

set(QTJACK_SPECTRUM_ANALYZER_APP_SOURCES
  src/qtjack_spectrum_analyzer_app.cpp
)

set(QTJACK_SPECTRUM_ANALYZER_WIDGET_SOURCES
  src/qtjack_spectrum_analyzer_widget.cpp
)
set(QTJACK_SPECTRUM_ANALYZER_WIDGET_HEADERS
  include/qtjack_spectrum_analyzer_widget.hpp
)

QT5_WRAP_CPP(QTJACK_SPECTRUM_ANALYZER_WIDGET_MOCrcs
  ${QTJACK_SPECTRUM_ANALYZER_WIDGET_HEADERS}
)

set(SPECTRUM_DISPLAY_WIDGET_SOURCES  
  src/spectrum_display_widget.cpp
)
set(SPECTRUM_DISPLAY_WIDGET_HEADERS
  include/spectrum_display_widget.hpp
)
QT5_WRAP_CPP(SPECTRUM_DISPLAY_WIDGET_MOCrcs
  ${SPECTRUM_DISPLAY_WIDGET_HEADERS}
)

add_executable(fftw_test)
target_sources(fftw_test
PRIVATE
  ${FFTW_TEST_SOURCES}
)
target_include_directories(fftw_test SYSTEM
PRIVATE
  ${FFTW3_INCLUDE_DIRS}
#  PkgConfig::FFTW
)
target_link_libraries(fftw_test 
PRIVATE
  ${FFTW3_LIBRARIES}
#  PkgConfig::FFTW
)

add_library(spectrum_analyzer)
target_sources(spectrum_analyzer
PRIVATE
  ${SPECTRUM_ANALYZER_SOURCES}
  ${SPECTRUM_ANALYZER_HEADERS}
)
target_include_directories(spectrum_analyzer SYSTEM
PRIVATE
  include
  ${FFTW3_INCLUDE_DIRS}
#  PkgConfig::FFTW
)
target_link_libraries(spectrum_analyzer
PRIVATE
  ${FFTW3_LIBRARIES}
#  PkgConfig::FFTW
)

add_executable(spectrum_analyzer_test)
target_sources(spectrum_analyzer_test
PRIVATE
  ${SPECTRUM_ANALYZER_TEST_SOURCES}
)
target_include_directories(spectrum_analyzer_test SYSTEM
PRIVATE
  include
)
target_link_libraries(spectrum_analyzer_test
PRIVATE
  spectrum_analyzer
)

add_library(spectrum_display_widget SHARED)
target_sources(spectrum_display_widget
PRIVATE
    ${SPECTRUM_DISPLAY_WIDGET_SOURCES}
    ${SPECTRUM_DISPLAY_WIDGET_HEADERS}
    ${SPECTRUM_DISPLAY_WIDGET_MOCrcs}
#    ${QTJACK_SPECTRUM_ANALYZER_WIDGET_UISrcs}
#    ${RCC_SOURCES}
)
target_include_directories(spectrum_display_widget SYSTEM
PUBLIC 
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/qtjack_spectrum_analyzer
PRIVATE
  include
  ${CMAKE_CURRENT_BINARY_DIR} 
)
target_link_libraries(spectrum_display_widget
PRIVATE
  Qt5::Widgets
  Qt5::Core
)
set_target_properties(spectrum_display_widget PROPERTIES PUBLIC_HEADER "${QTJACK_SPECTRUM_ANALYZER_WIDGET_HEADERS}")

add_library(qtjack_spectrum_analyzer_widget SHARED)
target_sources(qtjack_spectrum_analyzer_widget
PRIVATE
    ${QTJACK_SPECTRUM_ANALYZER_WIDGET_SOURCES}
    ${QTJACK_SPECTRUM_ANALYZER_WIDGET_HEADERS}
    ${QTJACK_SPECTRUM_ANALYZER_WIDGET_MOCrcs}
#    ${QTJACK_SPECTRUM_ANALYZER_WIDGET_UISrcs}
#    ${RCC_SOURCES}
)
target_include_directories(qtjack_spectrum_analyzer_widget SYSTEM
PUBLIC 
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/qtjack_spectrum_analyzer
PRIVATE
  include
  ${CMAKE_CURRENT_BINARY_DIR} 
)
target_link_libraries(qtjack_spectrum_analyzer_widget
PRIVATE
  qtjack::qtjack
  Qt5::Widgets
  Qt5::Core
)
set_target_properties(qtjack_spectrum_analyzer_widget PROPERTIES PUBLIC_HEADER "${QTJACK_SPECTRUM_ANALYZER_WIDGET_HEADERS}")

add_executable(qtjack_spectrum_analyzer_app)
target_sources(qtjack_spectrum_analyzer_app
PRIVATE
    ${QTJACK_SPECTRUM_ANALYZER_APP_SOURCES}
)
target_include_directories(qtjack_spectrum_analyzer_app SYSTEM
PUBLIC 
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/qtjack_spectrum_analyzer
PRIVATE
  include
  ${CMAKE_CURRENT_BINARY_DIR}   
)
target_link_libraries(qtjack_spectrum_analyzer_app
PRIVATE
  qtjack_spectrum_analyzer_widget
  Qt5::Core
  Qt5::Widgets
  qtjack::qtjack
)
