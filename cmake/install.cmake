# Installation configuration for xcmath
#
# This file handles the installation of xcmath library targets and headers.

include(CMakePackageConfigHelpers)

# Set uppercase project name
set(UP_XCMATH "XCMATH")

# Define install directory for CMake config
set(UP_XCMATH_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/xcmath")

# Install export set
install(
    TARGETS xcmath
    EXPORT xcmath
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/xcmath/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xcmath/
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
)

# Generate and install CMake config files
configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/xcmathConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/xcmathConfig.cmake
    INSTALL_DESTINATION ${UP_XCMATH_CMAKE_INSTALL_DIR}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/xcmathConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/xcmathConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/xcmathConfigVersion.cmake
    DESTINATION ${UP_XCMATH_CMAKE_INSTALL_DIR}
)

# Install export
install(
    EXPORT xcmath
    NAMESPACE xcmath::
    FILE xcmathConfig.cmake
    DESTINATION ${UP_XCMATH_CMAKE_INSTALL_DIR}
)
