// ** C1Sc Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C1_SC_H
#define _MZN_CMD_C1_SC_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class C1Sc : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, C1Sc const & cmd);

public:

    explicit C1Sc();
    ~C1Sc() = default;

    C1Sc(C1Sc && rhs) = default;
    C1Sc & operator=(C1Sc && rhs) = default;
    C1Sc(C1Sc const & rhs) = default;
    C1Sc & operator=(C1Sc const & rhs) = default;
    std::string const cmd_name = "c1_sc";

    CmdFieldIgnore<uint16_t> sensor_a_output_1_ignore;
    CmdField<bool, 1> sensor_a_output_1_active_high;
    BmSensorControlMap sensor_a_output_1_control_map;
    CmdFieldIgnore<uint16_t> sensor_a_output_2_ignore;
    CmdField<bool, 1> sensor_a_output_2_active_high;
    BmSensorControlMap sensor_a_output_2_control_map;
    CmdFieldIgnore<uint16_t> sensor_a_output_3_ignore;
    CmdField<bool, 1> sensor_a_output_3_active_high;
    BmSensorControlMap sensor_a_output_3_control_map;
    CmdFieldIgnore<uint16_t> sensor_a_output_4_ignore;
    CmdField<bool, 1> sensor_a_output_4_active_high;
    BmSensorControlMap sensor_a_output_4_control_map;
    CmdFieldIgnore<uint16_t> sensor_b_output_1_ignore;
    CmdField<bool, 1> sensor_b_output_1_active_high;
    BmSensorControlMap sensor_b_output_1_control_map;
    CmdFieldIgnore<uint16_t> sensor_b_output_2_ignore;
    CmdField<bool, 1> sensor_b_output_2_active_high;
    BmSensorControlMap sensor_b_output_2_control_map;
    CmdFieldIgnore<uint16_t> sensor_b_output_3_ignore;
    CmdField<bool, 1> sensor_b_output_3_active_high;
    BmSensorControlMap sensor_b_output_3_control_map;
    CmdFieldIgnore<uint16_t> sensor_b_output_4_ignore;
    CmdField<bool, 1> sensor_b_output_4_active_high;
    BmSensorControlMap sensor_b_output_4_control_map;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, C1Sc const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C1_SC_H
