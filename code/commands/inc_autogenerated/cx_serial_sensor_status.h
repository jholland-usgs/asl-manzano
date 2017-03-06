// ** CxSerialSensorStatus Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_CX_SERIAL_SENSOR_STATUS_H
#define _MZN_CMD_CX_SERIAL_SENSOR_STATUS_H

#include "command.h"


#include "cy_serial_sensor_status.h"

namespace mzn {

// -------------------------------------------------------------------------- //


enum class CxSerialSensorStatusKey {
    cy_serial_sensor_status = 0,
};
} // <- mzn
#include "command_container.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class CxSerialSensorStatus : public CommandContainer {

friend std::ostream & operator<<(std::ostream & cmd_os, CxSerialSensorStatus const & cmd);

public:

    explicit CxSerialSensorStatus();
    ~CxSerialSensorStatus() = default;

    CxSerialSensorStatus(CxSerialSensorStatus && rhs) = default;
    CxSerialSensorStatus & operator=(CxSerialSensorStatus && rhs) = default;
    std::string const cmd_name = "cx_serial_sensor_status";

    CmdField<uint16_t> total_size_of_this_block;
    CmdField<uint16_t> number_of_sub_blocks;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;
    // max keys implementation in a separate function/file
    // not part of auto generation
    uint16_t number_of_ic(std::vector<uint8_t> const & msg,
                          uint16_t mf_begin) const override;
    void create_new_ic(uint8_t const  cmd_key) override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, CxSerialSensorStatus const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_CX_SERIAL_SENSOR_STATUS_H
