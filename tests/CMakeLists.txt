add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

link_libraries(ssm)
link_libraries(Catch)

add_executable(vector-test vector.cpp)
add_executable(matrix-test matrix.cpp)
add_executable(quaternion-test quaternion.cpp)

add_test(vector vector-test)
add_test(matrix matrix-test)
add_test(quaternion quaternion-test)
