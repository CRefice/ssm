string(
    APPEND opts
    "$<IF:$<CXX_COMPILER_ID:MSVC>,"
        "/W3;$<$<CONFIG:RELEASE>:/O2>,"
				"-Wall;"
            "$<$<CONFIG:RELEASE>:-O3>"
            "$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>"
    ">"
)
add_compile_options("${opts}")
add_compile_options("-march=native")

add_library(catch catch.cpp)
target_link_libraries(catch PUBLIC Catch2)

link_libraries(ssm)
link_libraries(catch)

include(${PROJECT_SOURCE_DIR}/externals/Catch2/contrib/Catch.cmake)

add_executable(vector-test vector.cpp)
add_executable(matrix-test matrix.cpp)
add_executable(quaternion-test quaternion.cpp)
add_executable(transform-test transform.cpp)

catch_discover_tests(vector-test)
catch_discover_tests(matrix-test)
catch_discover_tests(quaternion-test)
catch_discover_tests(transform-test)
