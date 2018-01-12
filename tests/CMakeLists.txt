link_libraries(ssm)

add_executable(vector-test vector.cpp)
add_executable(matrix-test matrix.cpp)

add_test(vector vector-test)
add_test(matrix matrix-test)
