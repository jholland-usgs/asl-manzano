cmake_minimum_required(VERSION 3.0)
project(commands_config)

# CMake include guard
if(commands_config_included)
  return()
endif(commands_config_included)
set(commands_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../mzn_config.cmake)
include(${DIR_MZN}/cmd_field/cmd_field.cmake)

message("\n###### config(commands) #######")

set(DIR_COMMANDS ${DIR_MZN}/commands)

include_directories(${DIR_COMMANDS}/inc/)
include_directories(${DIR_COMMANDS}/inc_autogenerated/)

set(SOURCES_AG_COMMANDS  ${DIR_COMMANDS}/src_autogenerated/qdp_crc.cpp
                         ${DIR_COMMANDS}/src_autogenerated/qdp_header.cpp
                         ${DIR_COMMANDS}/src_autogenerated/dt_open.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_cack.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_cerr.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_ctrl.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_dev.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_dev.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_dsrv.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_glob.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_mysn.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_ping_2.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_ping_3.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_ping_4.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_ping_5.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_pollsn.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_rqdev.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_rqglob.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_rqsrv.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_rqstat.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_srvch.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_srvrsp.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c2_samass.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c2_rqamass.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c2_amass.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_boom_positions.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_global_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_gps_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_power_supply_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cy_thread_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_thread_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_pll_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cy_gps_satellites.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_gps_satellites.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cy_arp_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_arp_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_data_port_1_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_data_port_2_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_data_port_3_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_data_port_4_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_serial_interface_1_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_serial_interface_2_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_serial_interface_3_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_ethernet_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_baler_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_dynamic_ip.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_auxiliary_board_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cy_auxiliary_board_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_serial_sensor_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cy_serial_sensor_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cz_internal_temperature_measurement.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cz_humidity_and_external_temperature.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_environmental_processor_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cy_environmental_processor_status.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_stat.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_rqsc.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_sc.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_qcal.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c1_stop.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c2_rqqv.cpp
                         ${DIR_COMMANDS}/src_autogenerated/c2_qv.cpp
                         ${DIR_COMMANDS}/src_autogenerated/cx_qv.cpp
                         )

set(SOURCES_MCS ${DIR_COMMANDS}/src/mc_cx_thread_status.cpp
                ${DIR_COMMANDS}/src/mc_cx_gps_satellites.cpp
                ${DIR_COMMANDS}/src/mc_cx_arp_status.cpp
                ${DIR_COMMANDS}/src/mc_cx_auxiliary_board_status.cpp
                ${DIR_COMMANDS}/src/mc_cx_serial_sensor_status.cpp
                ${DIR_COMMANDS}/src/mc_cy_serial_sensor_status.cpp
                ${DIR_COMMANDS}/src/mc_cx_environmental_processor_status.cpp
                ${DIR_COMMANDS}/src/mc_c2_qv.cpp
                ${DIR_COMMANDS}/src/mc_c1_dev.cpp
                ${DIR_COMMANDS}/src/mc_c1_stat.cpp)

set(SOURCES_COMMANDS ${SOURCES_CMD_FIELD}
                     ${SOURCES_exceptions}
                     ${DIR_COMMANDS}/src/command.cpp
                     ${DIR_COMMANDS}/src/multi_command.cpp
                     ${DIR_COMMANDS}/src/multi_command_map.cpp
                     ${SOURCES_MCS}
                     ${SOURCES_AG_COMMANDS})
