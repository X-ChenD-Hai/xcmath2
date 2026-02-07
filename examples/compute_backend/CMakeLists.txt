add_executable(compute_backend_dev)

file(GLOB_RECURSE SOURCES *.cc)

target_sources(compute_backend_dev PRIVATE ${SOURCES})
target_link_libraries(compute_backend_dev PRIVATE xcmath)