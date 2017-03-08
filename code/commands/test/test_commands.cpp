#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <array>
#include <limits>
#include <cmath>

#include "gtest/gtest.h"
#include "command.h"
#include "command_container.h"
#include "command_map_ni.h"
#include "commands.h"
// -------------------------------------------------------------------------- //
class FixtureTokenMap : public ::testing::Test {
public:

    // mzn::TokenMap tokens{};
};

// -------------------------------------------------------------------------- //
// TEST_F(FixtureTokenMap, tokens_setup) {}

// -------------------------------------------------------------------------- //
class FixtureCommand : public ::testing::Test {
public:

    mzn::C1Cack c1_cack{};
    mzn::C1Cerr c1_cerr{};
    mzn::C1Ctrl c1_ctrl{};
    mzn::C1Dev c1_dev{};
    mzn::C1Dsrv c1_dsrv{};
    mzn::C1Mysn c1_mysn{};
    mzn::C1Ping2 c1_ping_2{};
    mzn::C1Ping3 c1_ping_3{};
    mzn::C1Ping4 c1_ping_4{};
    mzn::C1Ping5 c1_ping_5{};
    mzn::C1Pollsn c1_pollsn{};
    mzn::C1Qcal c1_qcal{};
    mzn::C1Rqdev c1_rqdev{};
    mzn::C1Rqsc c1_rqsc{};
    mzn::C1Rqsrv c1_rqsrv{};
    mzn::C1Rqstat c1_rqstat{};
    mzn::C1Sc c1_sc{};
    mzn::C1Srvch c1_srvch{};
    mzn::C1Srvrsp c1_srvrsp{};
    mzn::C1Stat c1_stat{};
    mzn::C1Stop c1_stop{};
    mzn::C2Qv c2_qv{};
    mzn::C2Rqqv c2_rqqv{};
    mzn::C2Samass c2_samass{};
    mzn::C2Rqamass c2_rqamass{};
    mzn::C2Amass c2_amass{};
    mzn::CxArpStatus cx_arp_status{};
    mzn::CxAuxiliaryBoardStatus cx_auxiliary_board_status{};
    mzn::CxBalerStatus cx_baler_status{};
    mzn::CxBoomPositions cx_boom_positions{};
    mzn::CxDataPort1Status cx_data_port_1_status{};
    mzn::CxDataPort2Status cx_data_port_2_status{};
    mzn::CxDataPort3Status cx_data_port_3_status{};
    mzn::CxDataPort4Status cx_data_port_4_status{};
    mzn::CxDev cx_dev{};
    mzn::CxDynamicIp cx_dynamic_ip{};
    mzn::CxEnvironmentalProcessorStatus cx_environmental_processor_status{};
    mzn::CxEthernetStatus cx_ethernet_status{};
    mzn::CxGlobalStatus cx_global_status{};
    mzn::CxGpsSatellites cx_gps_satellites{};
    mzn::CxGpsStatus cx_gps_status{};
    mzn::CxPllStatus cx_pll_status{};
    mzn::CxPowerSupplyStatus cx_power_supply_status{};
    mzn::CxQv cx_qv{};
    mzn::CxSerialInterface1Status cx_serial_interface_1_status{};
    mzn::CxSerialInterface2Status cx_serial_interface_2_status{};
    mzn::CxSerialInterface3Status cx_serial_interface_3_status{};
    mzn::CxSerialSensorStatus cx_serial_sensor_status{};
    mzn::CxThreadStatus cx_thread_status{};
    mzn::CyArpStatus cy_arp_status{};
    mzn::CyAuxiliaryBoardStatus cy_auxiliary_board_status{};
    mzn::CyEnvironmentalProcessorStatus cy_environmental_processor_status{};
    mzn::CyGpsSatellites cy_gps_satellites{};
    mzn::CySerialSensorStatus cy_serial_sensor_status{};
    mzn::CyThreadStatus cy_thread_status{};
    mzn::CzHumidityAndExternalTemperature cz_humidity_and_external_temperature{};
    mzn::CzInternalTemperatureMeasurement cz_internal_temperature_measurement{};
    mzn::QdpCrc qdp_crc{};
    mzn::QdpHeader qdp_header{};

    mzn::T1Ignore t1_ignore{};
    mzn::T1ClockProcessing t1_clock_processing{};
    mzn::T1ConfigurationIdentification t1_configuration_identification{};
    mzn::T1DataServer t1_data_server{};
    mzn::T1DataSubscriptionServer t1_data_subscription_server{};
    mzn::T1DpNetserver t1_dp_netserver{};
    mzn::T1DpWebserver t1_dp_webserver{};
    mzn::T1LogAndTiming t1_log_and_timing{};
    mzn::T1NetworkStation t1_network_station{};
    mzn::T1VersionNumber t1_version_number{};
    mzn::TxCommEvent tx_comm_event{};
    mzn::T2Tokens t2_tokens{};

    virtual void SetUp() {

        // assign test values and run a
        // simple test to confirm right name/type
        // and function call operator
    }
    //virtual void TearDown() {};
};

// fails only if SetUp() fails
// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, commands_setup) {}
void print_diff(std::vector<uint8_t> const & msg) {

    std::cout << std::hex << std::showbase << std::uppercase;
    std::cout << "[";
    for (auto const & element : msg) {
        std::cout << static_cast<unsigned int>(element) << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << std::dec << std::noshowbase << std::nouppercase;
}

// -------------------------------------------------------------------------- //
void print_diff(std::vector<uint8_t> const & msg1,
                std::vector<uint8_t> const & msg2) {

    if (msg1 != msg2) {
        std::cout << std::endl << "differences:\n";
        std::cout << "[\n";

        for (int i = 0; i < msg1.size(); i++) {

            if (static_cast<unsigned int>(msg1[i])
                != static_cast<unsigned int>(msg2[i])) {

                std::cout << "(i:" << i << ") ";
                std::cout << std::hex << std::showbase << std::uppercase
                          << static_cast<unsigned int>(msg1[i]) << ", "
                          << static_cast<unsigned int>(msg2[i]) << std::endl;
                std::cout << std::dec << std::noshowbase << std::nouppercase;
            }
        }
        std::cout << "]" << std::endl;
    }
}

// test some specific commands
// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, msg_to_data) {
    std::vector<uint8_t> msg = {
        79, 23, 165, 184,
        56, 2, 0, 72,
        0, 0, 0, 2, // end of header
        0, 5, 0, 0, // c1_ping_5 msg
        0, 1, 0, 31,
        0, 0, 6, 14,
        1, 0, 0, 11,
        105, 186, 23, 211,
        0, 32, 0, 0,
        0, 13, 0, 0,
        0, 6, 128, 0,
        0, 76, 127, 80,
        0, 16, 0, 0,
        0, 36, 193, 5,
        0, 0, 0, 0,
        0, 65, 189, 36,
        0, 0, 11, 144,
        0, 0, 0, 0,
        0, 0, 0, 3,
        0, 0, 0, 3,
        0, 0, 1, 146};

    c1_ping_5.msg_to_data(msg, 12);

    EXPECT_EQ( 5, c1_ping_5.ping_type() );
    EXPECT_EQ( 0, c1_ping_5.ignore_0() );
    EXPECT_EQ( 1, c1_ping_5.version() );

    EXPECT_EQ( true, c1_ping_5.fix_flags.ethernet_installed() );
    EXPECT_EQ( true, c1_ping_5.fix_flags.can_statreq_dynamic_ip() );
    EXPECT_EQ( true, c1_ping_5.fix_flags.can_statreq_auxiliary_board() );
    EXPECT_EQ( true, c1_ping_5.fix_flags.can_expanded_c1_web_cmds() );
    EXPECT_EQ( true, c1_ping_5.fix_flags.can_statreq_serial_sensor() );
    EXPECT_EQ( false, c1_ping_5.fix_flags.can_report_255maplus_supply_current() );
    EXPECT_EQ( false, c1_ping_5.fix_flags.at_least_one_environmental_proc_configured() );
    EXPECT_EQ( false, c1_ping_5.fix_flags.is_Q330S() );

    EXPECT_EQ( 1550, c1_ping_5.kmi_property_tag() );
    EXPECT_EQ( 0X100000B69BA17D3, c1_ping_5.serial_number() );
    EXPECT_EQ( 2097152, c1_ping_5.data_port_1_packet_memory_size() );
    EXPECT_EQ( 851968, c1_ping_5.data_port_2_packet_memory_size() );
    EXPECT_EQ( 425984, c1_ping_5.data_port_3_packet_memory_size() );
    EXPECT_EQ( 5013328, c1_ping_5.data_port_4_packet_memory_size() );
    EXPECT_EQ( 1048576, c1_ping_5.serial_interface_1_memory_trigger_level() );
    EXPECT_EQ( 2408709, c1_ping_5.serial_interface_2_memory_trigger_level() );
    EXPECT_EQ( 0, c1_ping_5.reserved_0() );
    EXPECT_EQ( 4308260, c1_ping_5.ethernet_interface_memory_trigger_level() );

    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.fill_mode_enabled() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags .flush_packet_buffer_time_based() );

    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.freeze_data_port_output() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.freeze_packet_buff_input() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.keep_oldest_data_packet_buffer() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.dp_piggyback_statreq_with_dt_dack() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.comm_fault_led_if_last_flush_buff_5plus() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.allow_hotswap_on_this_dataport() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.flush_sliding_window_buff_based_on_time() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.send_datapackets_encoded_base_96() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.save_changes_in_eeprom() );

    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.fill_mode_enabled() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.flush_packet_buffer_time_based() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.freeze_data_port_output() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.freeze_packet_buff_input() );
    EXPECT_EQ( true, c1_ping_5.serial_interface_2_advanced_flags.keep_oldest_data_packet_buffer() );
    EXPECT_EQ( true, c1_ping_5.serial_interface_2_advanced_flags.dp_piggyback_statreq_with_dt_dack() );
    EXPECT_EQ( true, c1_ping_5.serial_interface_2_advanced_flags.comm_fault_led_if_last_flush_buff_5plus() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.allow_hotswap_on_this_dataport() );
    EXPECT_EQ( true, c1_ping_5.serial_interface_2_advanced_flags.flush_sliding_window_buff_based_on_time() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.send_datapackets_encoded_base_96() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.save_changes_in_eeprom() );

    EXPECT_EQ( 0, c1_ping_5.reserved_1() );

    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.fill_mode_enabled() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.flush_packet_buffer_time_based() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.freeze_data_port_output() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.freeze_packet_buff_input() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.keep_oldest_data_packet_buffer() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.dp_piggyback_statreq_with_dt_dack() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.comm_fault_led_if_last_flush_buff_5plus() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.allow_hotswap_on_this_dataport() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.flush_sliding_window_buff_based_on_time() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.send_datapackets_encoded_base_96() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.save_changes_in_eeprom() );

    EXPECT_EQ( 0, c1_ping_5.serial_interface_1_data_port_number() );
    EXPECT_EQ( 3, c1_ping_5.serial_interface_2_data_port_number() );
    EXPECT_EQ( 0, c1_ping_5.reserved_2() );
    EXPECT_EQ( 3, c1_ping_5.ethernet_interface_data_port_number() );

    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_1_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_2_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_3_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_4_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_5_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_6_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_7_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_8_failed_calibration() );

    EXPECT_EQ( 1, c1_ping_5.firmware_major() );
    EXPECT_EQ( 146, c1_ping_5.firmware_minor() );

    std::vector<uint8_t> newmsg (84, 0);
    for (int i = 0; i < 12; i++) {
        newmsg[i] = msg[i];
    }
    c1_ping_5.data_to_msg(newmsg, 12);
    EXPECT_EQ(msg, newmsg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, serializing_c1_stat) {
    std::vector<uint8_t> msg = {
        0XED, 0XC5, 0X6C, 0X98, 0XA9, 0X2, 0X1, 0XE8, 0, 0, 0, 0X3, 0, 0, 0, 0X51,
        0, 0, 0, 0XC5, 0, 0, 0, 0X5, 0X1D, 0XC5, 0X4, 0X63, 0, 0, 0, 0, 0X13, 0XCA,
        0XCA, 0XBF, 0X10, 0XF7, 0X5E, 0XD9, 0X1D, 0XC5, 0X4, 0X67, 0, 0, 0X2, 0X46,
        0, 0, 0, 0, 0, 0, 0X8, 0XD1, 0, 0, 0, 0X3, 0, 0, 0, 0, 0, 0XC3, 0X69, 0X8E,
        0, 0X15, 0X1, 0X5C, 0, 0X2, 0XFB, 0X5C, 0XD, 0X5D, 0, 0, 0, 0, 0XB8, 0XEC,
        0X8D, 0X45, 0X1E, 0X1E, 0, 0X2, 0XFB, 0X5C, 0XD, 0X5D, 0X20, 0X9, 0, 0X1,
        0X27, 0X49, 0XEE, 0XEA, 0X1, 0X1, 0, 0X2, 0XFB, 0X5C, 0XD, 0X5D, 0, 0, 0,
        0, 0, 0XB, 0X4, 0X14, 0X8, 0X8, 0, 0X2, 0XFB, 0X5C, 0XD, 0X4C, 0X60, 0, 0,
        0, 0, 0XD, 0X11, 0XF4, 0X6, 0X6, 0, 0X2, 0XFB, 0X5C, 0XD, 0X44, 0X40, 0, 0,
        0, 0, 0XA, 0XDA, 0X54, 0X9, 0X9, 0, 0X2, 0XFB, 0X5C, 0XD, 0X34, 0X40, 0, 0,
        0, 0, 0, 0X22, 0X5C, 0X3, 0X3, 0, 0X2, 0XFB, 0X5B, 0XF0, 0X78, 0X40, 0, 0,
        0, 0, 0XD, 0XB2, 0X78, 0X7, 0X7, 0, 0X2, 0XFB, 0X5C, 0XD, 0X4C, 0X40, 0, 0,
        0, 0, 0X2C, 0X5B, 0XB3, 0XB, 0XB, 0, 0X2, 0XFB, 0X5C, 0XD, 0X43, 0X40, 0, 0,
        0, 0X5, 0X1B, 0X5B, 0X44, 0X14, 0X14, 0, 0X2, 0XFB, 0X5C, 0XC, 0X29, 0X20,
        0, 0, 0, 0, 0, 0X4C, 0XCE, 0XC, 0XC, 0, 0X2, 0XFB, 0X5C, 0XD, 0X42, 0X40, 0,
        0, 0, 0, 0X16, 0XE, 0X88, 0X12, 0X13, 0, 0X2, 0XFB, 0X5C, 0XC, 0X36, 0X20,
        0, 0, 0, 0X3, 0X5D, 0X74, 0XEC, 0X5, 0X5, 0, 0X2, 0XFB, 0X5C, 0XD, 0X5A,
        0X20, 0X1, 0, 0, 0, 0X53, 0X57, 0XB5, 0X10, 0X10, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X43, 0X40, 0, 0, 0, 0, 0, 0XAB, 0XC7, 0XF, 0XF, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X40, 0X40, 0, 0, 0, 0, 0, 0XD0, 0X92, 0XE, 0XE, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X40, 0X40, 0, 0, 0, 0, 0X1, 0XE1, 0X86, 0XD, 0XD, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X40, 0X40, 0, 0, 0, 0, 0X3, 0X9B, 0X9, 0X11, 0X11, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X43, 0X60, 0, 0, 0, 0, 0, 0, 0, 0XA, 0XA, 0, 0, 0, 0, 0, 0, 0X20, 0, 0, 0,
        0, 0, 0, 0, 0XA, 0XA, 0, 0, 0, 0, 0, 0, 0X20, 0, 0, 0, 0, 0, 0, 0, 0XA, 0XA,
        0, 0, 0, 0, 0, 0, 0X20, 0, 0, 0, 0, 0, 0, 0, 0X2, 0X2, 0, 0, 0, 0, 0, 0,
        0X80, 0, 0, 0XA, 0, 0X54, 0, 0X5, 0, 0X6, 0X1, 0X3A, 0, 0X29, 0, 0X7, 0,
        0X38, 0X1, 0X48, 0, 0X35, 0, 0X8, 0, 0X33, 0, 0X3, 0, 0, 0, 0X9, 0, 0X4A,
        0, 0XC4, 0, 0X2D, 0, 0XB, 0, 0XA, 0, 0X91, 0, 0, 0, 0X10, 0, 0XB, 0, 0X2E,
        0, 0, 0, 0X17, 0, 0X2C, 0, 0X9C, 0, 0, 0, 0X1B, 0, 0X23, 0, 0X3C, 0, 0, 0,
        0X1C, 0, 0X14, 0, 0XEB, 0, 0X30, 0, 0X1E, 0, 0X20, 0X1, 0X2B, 0, 0X30};

    c1_stat.msg_to_data(msg, 12);

    std::vector<uint8_t> newmsg (msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        newmsg[i] = msg[i];
    }

    c1_stat.data_to_msg(newmsg, 12);
    EXPECT_EQ(msg, newmsg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_dsrv_test) {
    mzn::C1Dsrv cmd{};
    std::vector<uint8_t> msg = {0XFB, 0X1, 0X3, 0X10,
                                0X12, 0X2, 0, 0X8,
                                0, 0X4, 0, 0,
                                0X1, 0, 0, 0XB,
                                0X69, 0XBA, 0X17, 0XD3};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_rqsrv_test) {
    mzn::C1Rqsrv cmd{};
    std::vector<uint8_t> msg = {0XEA, 0X8A, 0XB0, 0X88,
                                0X10, 0X2, 0, 0X8,
                                0, 0X1, 0, 0,
                                0X1, 0, 0, 0XB,
                                0X69, 0XBA, 0X17, 0XD3};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_srvrsp_test) {
    mzn::C1Srvrsp cmd{};
    std::vector<uint8_t> msg = {0X4D, 0X9E, 0X1E, 0X98,
                                0X11, 0X2, 0, 0X30,
                                0, 0X2, 0, 0,
                                0X1, 0, 0, 0XB,
                                0X69, 0XBA, 0X17, 0XD3,
                                0XD, 0XCD, 0X21, 0XE9,
                                0XE, 0XC5, 0XA7, 0XF6,
                                0X88, 0XB1, 0X79, 0X5D,
                                0X8, 0, 0X1, 0XC4,
                                0, 0, 0, 0,
                                0, 0X7A, 0X9F, 0X51,
                                0X44, 0XC1, 0X75, 0X70,
                                0X72, 0X22, 0X6B, 0XAE,
                                0X47, 0X56, 0XEF, 0XA2,
                                0X86, 0X87, 0X5C, 0X56};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_rqstat_test) {
    mzn::C1Rqstat cmd{};
    std::vector<uint8_t> msg = {0XED, 0, 0X8, 0X68,
                                0X1F, 0X2, 0, 0X4,
                                0, 0X3, 0, 0,
                                0, 0, 0X80, 0X51};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_rqsc_test) {
    mzn::C1Rqsc cmd{};
    std::vector<uint8_t> msg = {0X3D, 0X90, 0X9A, 0X98,
                                0X2F, 0X2, 0, 0,
                                0, 0X3, 0, 0};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_sc_test) {
    mzn::C1Sc cmd{};
    std::vector<uint8_t> msg = {0XCF, 0XE8, 0X2, 0XC0,
                                0XAF, 0X2, 0, 0X20,
                                0, 0, 0, 0X3,
                                0, 0, 0X1, 0X2,
                                0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0X1, 0X1,
                                0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0X1, 0X4};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_diff(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_qcal_test) {
    mzn::C1Qcal cmd{};
    std::vector<uint8_t> msg = {0XE8, 0X6A, 0X8B, 0XA8,
                                0X23, 0X2, 0, 0X24,
                                0, 0X3, 0, 0,
                                0, 0, 0, 0,
                                0, 0X80, 0, 0X1,
                                0, 0XA, 0, 0X5,
                                0, 0X38, 0, 0X5,
                                0, 0X80, 0, 0X2,
                                0, 0X1, 0, 0,
                                0X72, 0X65, 0X73, 0X69,
                                0X73, 0X74, 0X69, 0X76,
                                0X65, 0, 0, 0};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_diff(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_cack_test) {
    mzn::C1Cack cmd{};
    std::vector<uint8_t> msg = {0XE8, 0X6A, 0X8B, 0XA8,
                                0X23, 0X2, 0, 0X24,
                                0, 0X3, 0, 0};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_diff(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_pollsn_test) {
    mzn::C1Cack cmd{};
    std::vector<uint8_t> msg = {0X43, 0XBA, 0, 0XA8,
                                0X14, 0X2, 0, 0X4,
                                0, 0X1, 0, 0,
                                0, 0, 0, 0};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_diff(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, c1_rqdev_test) {
    mzn::C1Rqdev cmd{};
    std::vector<uint8_t> msg = {0X4D, 0XF9, 0X2E, 0X98,
                                0X36, 0X2, 0, 0,
                                0, 0X3, 0, 0};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_diff(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
template <typename T>
void token_test (std::vector<uint8_t> const & msg) {

    T cmd{};

    cmd.msg_to_data(msg, 0);
    std::cout << std::endl << cmd;
    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    cmd.data_to_msg(processed_msg, 0);

    print_diff(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, tx_comm_event) {

    std::vector<uint8_t> msg = {0x66, 0x06, 0x41, 0x4c,
                                0x4c, 0x43, 0x4f, 0x4d};

    token_test<mzn::TxCommEvent>(msg);
}


// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, t1_network_station) {

    // there are 13 TxCommEventName commands on this message,
    // event numbers from 00 to 0c
    // there are 0x5e (94) bytes of data including the 2 bytes of nb
    std::vector<uint8_t> msg =
        {0x58, 0x58, 0x44, 0x45, 0x56, 0x20, 0x20};

    token_test<mzn::T1NetworkStation>(msg);
}
// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, t1_comm_event) {

    // there are 13 TxCommEventName commands on this message,
    // event numbers from 00 to 0c
    // there are 0x5e (94) bytes of data including the 2 bytes of nb
    std::vector<uint8_t> msg =
        {0x00, 0x5e, 0x00, 0x06, 0x41, 0x4c, 0x4c, 0x43,
         0x4f, 0x4d, 0x01, 0x03, 0x48, 0x4f, 0x4e, 0x02,
         0x03, 0x42, 0x4f, 0x4e, 0x03, 0x03, 0x4d, 0x4f,
         0x4e, 0x04, 0x03, 0x4c, 0x4f, 0x4e, 0x05, 0x06,
         0x48, 0x2e, 0x42, 0x4f, 0x46, 0x46, 0x06, 0x06,
         0x48, 0x2e, 0x41, 0x4f, 0x46, 0x46, 0x07, 0x06,
         0x42, 0x2e, 0x42, 0x4f, 0x46, 0x46, 0x08, 0x06,
         0x42, 0x2e, 0x41, 0x4f, 0x46, 0x46, 0x09, 0x06,
         0x4d, 0x2e, 0x42, 0x4f, 0x46, 0x46, 0x0a, 0x06,
         0x4d, 0x2e, 0x41, 0x4f, 0x46, 0x46, 0x0b, 0x06,
         0x4c, 0x2e, 0x42, 0x4f, 0x46, 0x46, 0x0c, 0x06,
         0x4c, 0x2e, 0x41, 0x4f, 0x46, 0x46};

    token_test<mzn::T1CommEvent>(msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, tx_irr_filter) {

    std::vector<uint8_t> msg = {0x3d, 0x43, 0x0c, 0x24, 0x82};
    std::vector<uint8_t> msg2 = {0x3d, 0x88, 0x88, 0x8d, 0x02};

    token_test<mzn::TxIrrFilter>(msg);
    token_test<mzn::TxIrrFilter>(msg2);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, t1_irr_filter) {

    std::vector<uint8_t> msg = {
        0x1b, 0x01, 0x05, 0x56, 0x53, 0x50, 0x42, 0x50, 0x02,
        0x41, 0x20, 0x00, 0x00, 0x42, 0xa0, 0x00, 0x00,
        0x3d, 0x43, 0x0c, 0x24, 0x82,
        0x3d, 0x88, 0x88, 0x8d, 0x02,
    };
    std::vector<uint8_t> msg2 = {
        0x1e, 0x02, 0x08, 0x56, 0x53, 0x50, 0x48, 0x31, 0x4c, 0x31, 0x30, 0x02,
        0x3f, 0x80, 0x00, 0x00, 0x42, 0xa0, 0x00, 0x00,
        0x3c, 0x4c, 0xcc, 0xcd, 0x82,
        0x3e, 0x00, 0x00, 0x00, 0x02,
    };

    token_test<mzn::T1IrrFilter>(msg);
    token_test<mzn::T1IrrFilter>(msg2);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, tx_fir_filter) {

    std::vector<uint8_t> msg = {0x01, 0x06, 0x56, 0x4c, 0x50, 0x33, 0x38, 0x39};
    token_test<mzn::TxFirFilter>(msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, t1_fir_filter) {

    std::vector<uint8_t> msg = {
        0x09, 0x01, 0x06, 0x56, 0x4c, 0x50, 0x33, 0x38, 0x39,
    };

    token_test<mzn::T1FirFilter>(msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, t1_threshold_detector) {

    std::vector<uint8_t> msg = {
        0x1a, 0x00, 0xff, 0x02, 0x05, 0x00, 0x00, 0x00,
        0x55, 0xff, 0xff, 0xff, 0xd8, 0x00, 0x32, 0x00,
        0x00, 0x08, 0x4f, 0x56, 0x45, 0x52, 0x54, 0x45,
        0x4d, 0x50,
    };

    token_test<mzn::T1ThresholdDetector>(msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, t1_murdock_hutt_detector) {

    std::vector<uint8_t> msg = {
              0x24, 0x02, 0x0a, 0xa0, 0x04, 0x00, 0x00,
        0x00, 0x02, 0x00, 0x00, 0x00, 0x28, 0x0b, 0xb8,
        0x00, 0x00, 0x01, 0xf4, 0x21, 0x17, 0x04, 0x0d,
        0x08, 0x0b, 0x53, 0x45, 0x49, 0x53, 0x5f, 0x48,
        0x5f, 0x52, 0x41, 0x54, 0x45,
    };

    token_test<mzn::T1MurdockHuttDetector>(msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, t2_tokens) {

    std::vector<uint8_t> msg = {
        0x01, 0x00,

        0x02, 0x58, 0x58, 0x44, 0x45, 0x56, 0x20, 0x20,

        0x03, 0x00, 0x00,

        0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        0x05, 0x15, 0x04,

        0x09, 0x00, 0x00,

        0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x64,
        0x5a, 0x50, 0x50, 0x00, 0x3c, 0x0a, 0x00, 0x00, 0x14,

        0x07, 0x20, 0x20, 0x4c, 0x4f, 0x47, 0x20, 0x20, 0x41, 0x43, 0x45,

        0x08, 0x20, 0x20, 0x4f, 0x43, 0x46, 0x07, 0x05, 0xa0,

        0xc0, 0x00, 0x5e, 0x00, 0x06, 0x41, 0x4c, 0x4c,
        0x43, 0x4f, 0x4d, 0x01, 0x03, 0x48, 0x4f, 0x4e,
        0x02, 0x03, 0x42, 0x4f, 0x4e, 0x03, 0x03, 0x4d,
        0x4f, 0x4e, 0x04, 0x03, 0x4c, 0x4f, 0x4e, 0x05,
        0x06, 0x48, 0x2e, 0x42, 0x4f, 0x46, 0x46, 0x06,
        0x06, 0x48, 0x2e, 0x41, 0x4f, 0x46, 0x46, 0x07,
        0x06, 0x42, 0x2e, 0x42, 0x4f, 0x46, 0x46, 0x08,
        0x06, 0x42, 0x2e, 0x41, 0x4f, 0x46, 0x46, 0x09,
        0x06, 0x4d, 0x2e, 0x42, 0x4f, 0x46, 0x46, 0x0a,
        0x06, 0x4d, 0x2e, 0x41, 0x4f, 0x46, 0x46, 0x0b,
        0x06, 0x4c, 0x2e, 0x42, 0x4f, 0x46, 0x46, 0x0c,
        0x06, 0x4c, 0x2e, 0x41, 0x4f, 0x46, 0x46,

        0x81, 0x1c, 0x00, 0x06, 0x53, 0x50, 0x57, 0x57,
        0x53, 0x53, 0x02, 0x41, 0x20, 0x00, 0x00, 0x41,
        0xa0, 0x00, 0x00, 0x3d, 0x43, 0x0c, 0x24, 0x82,
        0x3d, 0x88, 0x88, 0x8d, 0x02,

        0x81, 0x1b, 0x01, 0x05, 0x56, 0x53, 0x50, 0x42,
        0x50, 0x02, 0x41, 0x20, 0x00, 0x00, 0x42, 0xa0,
        0x00, 0x00, 0x3d, 0x43, 0x0c, 0x24, 0x82, 0x3d,
        0x88, 0x88, 0x8d, 0x02,

        0x81, 0x1e, 0x02, 0x08, 0x56, 0x53, 0x50, 0x48,
        0x31, 0x4c, 0x31, 0x30, 0x02, 0x3f, 0x80, 0x00,
        0x00, 0x42, 0xa0, 0x00, 0x00, 0x3c, 0x4c, 0xcc,
        0xcd, 0x82, 0x3e, 0x00, 0x00, 0x00, 0x02,

        0x81, 0x17, 0x03, 0x06, 0x56, 0x53, 0x50, 0x31,
        0x35, 0x4c, 0x01, 0x3f, 0x80, 0x00, 0x00, 0x42,
        0xc8, 0x00, 0x00, 0x3e, 0x19, 0x99, 0x9a, 0x02,

        0x81, 0x1c, 0x04, 0x06, 0x4c, 0x50, 0x57, 0x57,
        0x53, 0x53, 0x02, 0x3f, 0x80, 0x00, 0x00, 0x3f,
        0x80, 0x00, 0x00, 0x3c, 0x23, 0xd7, 0x0a, 0x82,
        0x3d, 0x88, 0x88, 0x8d, 0x02,

        0x81, 0x1b, 0x05, 0x05, 0x4c, 0x50, 0x53, 0x52,
        0x4f, 0x02, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x80,
        0x00, 0x00, 0x3c, 0xa3, 0xd7, 0x0a, 0x82, 0x3d,
        0x23, 0xd7, 0x0a, 0x06,

        0x81, 0x1b, 0x06, 0x05, 0x56, 0x4c, 0x50, 0x42,
        0x50, 0x02, 0x3f, 0x80, 0x00, 0x00, 0x3d, 0xcc,
        0xcc, 0xcd, 0x3c, 0x88, 0x88, 0x9a, 0x82, 0x3d,
        0x88, 0x88, 0x2f, 0x02,

        0x81, 0x17, 0x07, 0x06, 0x56, 0x4c, 0x50, 0x41,
        0x56, 0x47, 0x01, 0x3f, 0x80, 0x00, 0x00, 0x3d,
        0xcc, 0xcc, 0xcd, 0x3c, 0x88, 0x88, 0x65, 0x82,

        0x81, 0x17, 0x08, 0x06, 0x55, 0x4c, 0x50, 0x41,
        0x56, 0x47, 0x01, 0x3f, 0x80, 0x00, 0x00, 0x3c,
        0x23, 0xd7, 0x0a, 0x3c, 0x88, 0x88, 0x9a, 0x82,

        0x82, 0x09, 0x01, 0x06, 0x56, 0x4c, 0x50, 0x33, 0x38, 0x39,
        0x82, 0x09, 0x02, 0x06, 0x55, 0x4c, 0x50, 0x33, 0x37, 0x39,

        0x85, 0x1a, 0x00, 0xff, 0x02, 0x05, 0x00, 0x00,
        0x00, 0x55, 0xff, 0xff, 0xff, 0xd8, 0x00, 0x32,
        0x00, 0x00, 0x08, 0x4f, 0x56, 0x45, 0x52, 0x54,
        0x45, 0x4d, 0x50,

        0x85, 0x18, 0x01, 0xff, 0x01, 0x02, 0x00, 0x00,
        0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14,
        0x00, 0x00, 0x06, 0x41, 0x4e, 0x54, 0x43, 0x55,
        0x52,

        0x84, 0x24, 0x02, 0x0a, 0xa0, 0x04, 0x00, 0x00,
        0x00, 0x02, 0x00, 0x00, 0x00, 0x28, 0x0b, 0xb8,
        0x00, 0x00, 0x01, 0xf4, 0x21, 0x17, 0x04, 0x0d,
        0x08, 0x0b, 0x53, 0x45, 0x49, 0x53, 0x5f, 0x48,
        0x5f, 0x52, 0x41, 0x54, 0x45,

        0x84, 0x23, 0x03, 0x0a, 0xa0, 0x04, 0x00, 0x00,
        0x00, 0x02, 0x00, 0x00, 0x00, 0x28, 0x0b, 0xb8,
        0x00, 0x00, 0x01, 0xf4, 0x21, 0x17, 0x04, 0x0d,
        0x08, 0x0a, 0x46, 0x42, 0x41, 0x5f, 0x48, 0x5f,
        0x52, 0x41, 0x54, 0x45,
    };

    token_test<mzn::T2Tokens>(msg);
}
// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, cmd_code) {
    std::stringstream ss;
    ss << "c1_cack : " << c1_cack << std::endl;
    ss << "c1_cerr : " << c1_cerr << std::endl;
    ss << "c1_ctrl : " << c1_ctrl << std::endl;
    ss << "c1_dev : " << c1_dev << std::endl;
    ss << "c1_dsrv : " << c1_dsrv << std::endl;
    ss << "c1_mysn : " << c1_mysn << std::endl;
    ss << "c1_ping_2 : " << c1_ping_2 << std::endl;
    ss << "c1_ping_3 : " << c1_ping_3 << std::endl;
    ss << "c1_ping_4 : " << c1_ping_4 << std::endl;
    ss << "c1_ping_5 : " << c1_ping_5 << std::endl;
    ss << "c1_pollsn : " << c1_pollsn << std::endl;
    ss << "c1_qcal : " << c1_qcal << std::endl;
    ss << "c1_rqdev : " << c1_rqdev << std::endl;
    ss << "c1_rqsc : " << c1_rqsc << std::endl;
    ss << "c1_rqsrv : " << c1_rqsrv << std::endl;
    ss << "c1_rqstat : " << c1_rqstat << std::endl;
    ss << "c1_sc : " << c1_sc << std::endl;
    ss << "c1_srvch : " << c1_srvch << std::endl;
    ss << "c1_srvrsp : " << c1_srvrsp << std::endl;
    ss << "c1_stat : " << c1_stat << std::endl;
    ss << "c1_stop : " << c1_stop << std::endl;
    ss << "c2_qv : " << c2_qv << std::endl;
    ss << "c2_rqqv : " << c2_rqqv << std::endl;
    ss << "cx_arp_status : " << cx_arp_status << std::endl;
    ss << "cx_auxiliary_board_status : " << cx_auxiliary_board_status << std::endl;
    ss << "cx_baler_status : " << cx_baler_status << std::endl;
    ss << "cx_boom_positions : " << cx_boom_positions << std::endl;
    ss << "cx_data_port_1_status : " << cx_data_port_1_status << std::endl;
    ss << "cx_data_port_2_status : " << cx_data_port_2_status << std::endl;
    ss << "cx_data_port_3_status : " << cx_data_port_3_status << std::endl;
    ss << "cx_data_port_4_status : " << cx_data_port_4_status << std::endl;
    ss << "cx_dev : " << cx_dev << std::endl;
    ss << "cx_dynamic_ip : " << cx_dynamic_ip << std::endl;
    ss << "cx_environmental_processor_status : " << cx_environmental_processor_status << std::endl;
    ss << "cx_ethernet_status : " << cx_ethernet_status << std::endl;
    ss << "cx_global_status : " << cx_global_status << std::endl;
    ss << "cx_gps_satellites : " << cx_gps_satellites << std::endl;
    ss << "cx_gps_status : " << cx_gps_status << std::endl;
    ss << "cx_pll_status : " << cx_pll_status << std::endl;
    ss << "cx_power_supply_status : " << cx_power_supply_status << std::endl;
    ss << "cx_qv : " << cx_qv << std::endl;
    ss << "cx_serial_interface_1_status : " << cx_serial_interface_1_status << std::endl;
    ss << "cx_serial_interface_2_status : " << cx_serial_interface_2_status << std::endl;
    ss << "cx_serial_interface_3_status : " << cx_serial_interface_3_status << std::endl;
    ss << "cx_serial_sensor_status : " << cx_serial_sensor_status << std::endl;
    ss << "cx_thread_status : " << cx_thread_status << std::endl;
    ss << "cy_arp_status : " << cy_arp_status << std::endl;
    ss << "cy_auxiliary_board_status : " << cy_auxiliary_board_status << std::endl;
    ss << "cy_environmental_processor_status : " << cy_environmental_processor_status << std::endl;
    ss << "cy_gps_satellites : " << cy_gps_satellites << std::endl;
    ss << "cy_serial_sensor_status : " << cy_serial_sensor_status << std::endl;
    ss << "cy_thread_status : " << cy_thread_status << std::endl;
    ss << "cz_humidity_and_external_temperature : " << cz_humidity_and_external_temperature << std::endl;
    ss << "cz_internal_temperature_measurement : " << cz_internal_temperature_measurement << std::endl;
    ss << "qdp_crc : " << qdp_crc << std::endl;
    ss << "qdp_header : " << qdp_header << std::endl;
}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
