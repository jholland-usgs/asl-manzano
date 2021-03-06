// ** CxSerialInterface3Status Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_CX_SERIAL_INTERFACE_3_STATUS_H
#define _MZN_CMD_CX_SERIAL_INTERFACE_3_STATUS_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class CxSerialInterface3Status : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, CxSerialInterface3Status const & cmd);

public:

    explicit CxSerialInterface3Status();
    ~CxSerialInterface3Status() = default;

    CxSerialInterface3Status(CxSerialInterface3Status && rhs) = default;
    CxSerialInterface3Status & operator=(CxSerialInterface3Status && rhs) = default;
    CxSerialInterface3Status(CxSerialInterface3Status const & rhs) = default;
    CxSerialInterface3Status & operator=(CxSerialInterface3Status const & rhs) = default;
    std::string const cmd_name = "cx_serial_interface_3_status";

    CmdField<uint32_t> receive_checksum_errors;
    CmdField<uint32_t> total_io_errors;
    CmdField<uint16_t> physical_interface_number;
    CmdFieldIgnore<uint16_t> spare_0;
    CmdField<uint32_t> destination_unreachable_icmp_packets_received;
    CmdField<uint32_t> source_quench_icmp_packets_received;
    CmdField<uint32_t> echo_request_icmp_packets_received;
    CmdField<uint32_t> redirect_icmp_packets_received;
    CmdField<uint32_t> character_overruns;
    CmdField<uint32_t> framing_errors;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, CxSerialInterface3Status const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_CX_SERIAL_INTERFACE_3_STATUS_H
