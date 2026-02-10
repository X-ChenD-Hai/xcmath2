
find_program(FMT_EXE NAMES "clang-format")

set(SOURCE_DIRS "test" "examples" "xcmath")
set(SOURCES)

foreach(DIR  ${SOURCE_DIRS})
    file(GLOB_RECURSE FILES
     ${CMAKE_SOURCE_DIR}/${DIR}/*.cc ${CMAKE_SOURCE_DIR}/${DIR}/*.hpp
    )
    list(APPEND SOURCES ${FILES})
endforeach()

if(FMT_EXE)
    add_custom_target(format
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${FMT_EXE}
        -i
        ${SOURCES}
    )
endif()
