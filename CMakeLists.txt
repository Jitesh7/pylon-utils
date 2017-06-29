# swatbotics/apriltags-cpp/CMakeLists.txt

cmake_minimum_required(VERSION 2.6)

project(PYLONUTILS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if(APPLE)
  include_directories(/opt/local/include) # MacPorts
  link_directories(/opt/local/lib)
endif()

include(FindPkgConfig)

set(BASLER_PYLON_INCLUDE_PATH /opt/pylon5/include)
set(BASLER_PYLON_LIBRARY pylonbase pylonutility GenApi_gcc_v3_0_Basler_pylon_v5_0 GCBase_gcc_v3_0_Basler_pylon_v5_0)
link_directories(/opt/pylon5/lib64)

pkg_search_module(OPENCV REQUIRED opencv>=2.3 opencv-2.3.1)
include_directories(${OPENCV_INCLUDE_DIRS})
include_directories(${BASLER_PYLON_INCLUDE_PATH})

add_subdirectory(src)
