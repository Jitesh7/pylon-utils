# TODO: stuff

add_library(basler_utils basler_opencv_utils.cpp basler_utils.cpp image_utils.cpp)

set(libs basler_utils ${BASLER_PYLON_LIBRARY} ${OPENCV_LIBRARIES} ${JPEG_LIBRARY} ${PNG_LIBRARY})

add_executable(list-params list-params.cpp)
target_link_libraries(list-params ${libs})

add_executable(cvdemo cvdemo.cpp)
target_link_libraries(cvdemo ${libs})

add_executable(dump_raw dump_raw.cpp)
target_link_libraries(dump_raw ${libs})

add_executable(test_encoding test_encoding.cpp)
target_link_libraries(test_encoding ${libs})
