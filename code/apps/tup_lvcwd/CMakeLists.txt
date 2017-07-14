cmake_minimum_required(VERSION 3.0)
project(tup_lvcwd)

include(tup_lvcwd.cmake)

message("\n###### tup_lvcwd #######")

add_executable(tup_lvcwd ${SOURCES_tup_lvcwd})
install(TARGETS tup_lvcwd DESTINATION /usr/local/bin/)

## serial is built as a library in serial_config.cmake
add_dependencies(tup_lvcwd serial_library)
target_link_libraries(tup_lvcwd serial_library)

if(APPLE)
    message("\n!!! serial_library not set to build for apple !!!")
endif()
