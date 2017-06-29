# TODO: stuff
add_executable(Grab Grab.cpp)
target_link_libraries(Grab ${BASLER_PYLON_LIBRARY})

add_executable(list-params list-params.cpp)
target_link_libraries(list-params ${BASLER_PYLON_LIBRARY})

add_executable(cvdemo cvdemo.cpp)
target_link_libraries(cvdemo ${BASLER_PYLON_LIBRARY} ${OPENCV_LIBRARIES})

