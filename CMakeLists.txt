# swatbotics/apriltags-cpp/CMakeLists.txt

cmake_minimum_required(VERSION 2.6)

project(PYLONUTILS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_library(JPEG_LIBRARY jpeg)
find_library(PNG_LIBRARY png)

if(APPLE)
  include_directories(/opt/local/include) # MacPorts
  link_directories(/opt/local/lib)
  find_library(BASLER_PYLON_LIBRARY pylon)
  mark_as_advanced(BASLER_PYLON_LIBRARY)
  include_directories(/Library/Frameworks/pylon.framework/Headers/GenICam)
else()
  set(BASLER_PYLON_INCLUDE_PATH /opt/pylon5/include)
  set(BASLER_PYLON_LIBRARY pylonbase pylonutility GenApi_gcc_v3_0_Basler_pylon_v5_0 GCBase_gcc_v3_0_Basler_pylon_v5_0)
  link_directories(/opt/pylon5/lib64 /opt/pylon5/lib)
  include_directories(${BASLER_PYLON_INCLUDE_PATH})
endif()

include(FindPkgConfig)


pkg_search_module(OPENCV REQUIRED opencv>=2.3 opencv-2.3.1)
include_directories(${OPENCV_INCLUDE_DIRS})

add_subdirectory(src)
