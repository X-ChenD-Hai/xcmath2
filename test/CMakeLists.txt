include(${CMAKE_SOURCE_DIR}/cmake/include_test_deps.cmake)
set(TEST_LIBS
    xcmath
    GTest::gtest GTest::gtest_main
)
add_executable(vec_test)

target_sources(vec_test PRIVATE
    vec/constructor_test.cc
    vec/accessor_test.cc
    vec/method_test.cc
    vec/view_test.cc
    vec/properties_test.cc
    vec/linear_algebra_test.cc
    vec/interpolation_test.cc
)

target_link_libraries(
    vec_test
    PRIVATE
    ${TEST_LIBS}
)

add_test(
    NAME vec_test
    COMMAND vec_test
)

# Matrix tests
add_executable(mat_test)

target_sources(mat_test PRIVATE
    mat/operations_test.cc
    mat/operations_row_major_test.cc
    mat/operations_col_major_test.cc
    mat/transform_test.cc
)

target_link_libraries(
    mat_test
    PRIVATE
    ${TEST_LIBS}
)

add_test(
    NAME mat_test
    COMMAND mat_test
)
