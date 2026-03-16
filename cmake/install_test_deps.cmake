# Test dependency installation for xcmath
#
# This file handles the installation of test dependencies (Google Test).

include(FetchContent)

# Google Test
if(NOT gtest_PRESENT)
    message(STATUS "Fetching Google Test...")
    FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    # For Windows: prevent GDTHREAD local conflict
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(gtest)
endif()

# Set test support libraries
set(TEST_SUPPORT_LIBRARIES gtest gtest_main)
