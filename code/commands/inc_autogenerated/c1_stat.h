// ** C1Stat Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C1_STAT_H
#define _MZN_CMD_C1_STAT_H

#include "command.h"


#include "cx_global_status.h"
#include "cx_gps_status.h"
#include "cx_power_supply_status.h"
#include "cx_boom_positions.h"
#include "cx_thread_status.h"
#include "cx_pll_status.h"
#include "cx_gps_satellites.h"
#include "cx_arp_status.h"
#include "cx_data_port_1_status.h"
#include "cx_data_port_2_status.h"
#include "cx_data_port_3_status.h"
#include "cx_data_port_4_status.h"
#include "cx_serial_interface_1_status.h"
#include "cx_serial_interface_2_status.h"
#include "cx_serial_interface_3_status.h"
#include "cx_ethernet_status.h"
#include "cx_baler_status.h"
#include "cx_dynamic_ip.h"
#include "cx_auxiliary_board_status.h"
#include "cx_serial_sensor_status.h"
#include "cx_environmental_processor_status.h"

#define k_cx_global_status 0
#define k_cx_gps_status 1
#define k_cx_power_supply_status 2
#define k_cx_boom_positions 3
#define k_cx_thread_status 4
#define k_cx_pll_status 5
#define k_cx_gps_satellites 6
#define k_cx_arp_status 7
#define k_cx_data_port_1_status 8
#define k_cx_data_port_2_status 9
#define k_cx_data_port_3_status 10
#define k_cx_data_port_4_status 11
#define k_cx_serial_interface_1_status 12
#define k_cx_serial_interface_2_status 13
#define k_cx_serial_interface_3_status 14
#define k_cx_ethernet_status 15
#define k_cx_baler_status 16
#define k_cx_dynamic_ip 17
#define k_cx_auxiliary_board_status 18
#define k_cx_serial_sensor_status 19
#define k_cx_environmental_processor_status 20

#include "multi_command_map.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class C1Stat : public MultiCommandMap {

friend std::ostream & operator<<(std::ostream & cmd_os, C1Stat const & cmd);

public:

    explicit C1Stat();
    ~C1Stat() = default;

    C1Stat(C1Stat && rhs) = default;
    C1Stat & operator=(C1Stat && rhs) = default;
    std::string const cmd_name = "c1_stat";

    BmStatStatusBitmap status_bitmap;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;
    bool command_active(uint8_t const cmd_key) const override;
    // max keys known at compile time for fixed maps mc
    uint16_t number_of_ic(std::vector<uint8_t> const & msg,
                          uint16_t mf_begin) const override {
        return 21;
    }

    void create_new_ic(uint8_t const  cmd_key) override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, C1Stat const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C1_STAT_H
