cmake_minimum_required(VERSION 3.15)
project(xcmath LANGUAGES CXX C VERSION 0.1.0)

option(BUILD_TESTING "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

if(BUILD_TESTING)
    enable_testing()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

include(${CMAKE_SOURCE_DIR}/cmake/format.cmake)
add_subdirectory(third_party/xcmixin)
add_subdirectory(xcmath)

if(${BUILD_EXAMPLES})
    add_subdirectory(examples/usage)
endif()

if(${BUILD_TESTING})
    add_subdirectory(test)
endif()

install(TARGETS xcmath EXPORT xcmath)

install(
    EXPORT xcmath
    NAMESPACE xcmath::
    FILE xcmathConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/xcmath
)
install(
    DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/xcmath/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xcmath/
    FILES_MATCHING
    PATTERN "*.hpp"
)
