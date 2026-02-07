find_package(GTest REQUIRED CONFIG)
enable_testing()
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
