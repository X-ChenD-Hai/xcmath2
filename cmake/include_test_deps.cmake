find_package(GTest CONFIG)

if(NOT GTest_FOUND)
    set(CMAKE_CXX_FLAGS_TMP "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-character-conversion")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_TMP}")
endif()
