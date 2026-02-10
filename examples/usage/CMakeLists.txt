add_executable(usage)

file(GLOB_RECURSE SOURCES *.cc)

target_sources(usage PRIVATE ${SOURCES})
target_link_libraries(usage PRIVATE xcmath)