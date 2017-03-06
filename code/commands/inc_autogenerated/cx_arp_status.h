// ** CxArpStatus Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_CX_ARP_STATUS_H
#define _MZN_CMD_CX_ARP_STATUS_H

#include "command.h"


#include "cy_arp_status.h"

namespace mzn {

// -------------------------------------------------------------------------- //


enum class CxArpStatusKey {
    cy_arp_status = 0,
};
} // <- mzn
#include "command_container.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class CxArpStatus : public CommandContainer {

friend std::ostream & operator<<(std::ostream & cmd_os, CxArpStatus const & cmd);

public:

    explicit CxArpStatus();
    ~CxArpStatus() = default;

    CxArpStatus(CxArpStatus && rhs) = default;
    CxArpStatus & operator=(CxArpStatus && rhs) = default;
    std::string const cmd_name = "cx_arp_status";

    CmdField<uint16_t> number_of_entries;
    CmdField<uint16_t> size_of_this_block;

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


inline std::ostream & operator<<(std::ostream & cmd_os, CxArpStatus const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_CX_ARP_STATUS_H
