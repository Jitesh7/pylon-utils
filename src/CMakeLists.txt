# TODO: stuff

add_library(basler_utils basler_utils.cpp ycbcr_to_jpeg.cpp)

add_executable(list-params list-params.cpp)
target_link_libraries(list-params basler_utils ${BASLER_PYLON_LIBRARY})

add_executable(cvdemo cvdemo.cpp)
target_link_libraries(cvdemo ${BASLER_PYLON_LIBRARY} ${OPENCV_LIBRARIES})

add_executable(dump_raw dump_raw.cpp)
target_link_libraries(dump_raw  basler_utils ${BASLER_PYLON_LIBRARY})

add_executable(test_ycbcr test_ycbcr.cpp)
target_link_libraries(test_ycbcr basler_utils ${JPEG_LIBRARY} ${BASLER_PYLON_LIBRARY} ${OPENCV_LIBRARIES})
