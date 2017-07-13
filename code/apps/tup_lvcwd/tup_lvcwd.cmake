cmake_minimum_required(VERSION 3.0)
project(tup_lvcwd_config)

# CMake include guard
if(tup_lvcwd_config_included)
  return()
endif(tup_lvcwd_config_included)
set(tup_lvcwd_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../mzn_config.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../serial_config.cmake)

include(${DIR_MZN}/connection/connection.cmake)

message("\n###### config(tup_lvcwd) #######")

set(DIR_tup_lvcwd ${DIR_MZN}/apps/tup_lvcwd)
include_directories(${DIR_tup_lvcwd}/inc)

set(SOURCES_tup_lvcwd ${SOURCES_connection}
                      ${DIR_connection}/src/serial_connection.cpp
                      ${DIR_tup_lvcwd}/src/tup_lvcwd.cpp)

