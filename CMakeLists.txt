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

set(QTJACK_SPECTRUM_ANALYZER_APP_SOURCES
  src/qtjack_spectrum_analyzer_app.cpp
)

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
#  qtjack_spectrum_analyzer
  Qt5::Core
  Qt5::Widgets
  qtjack::qtjack
)