// ** C1Rqmem Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C1_RQMEM_H
#define _MZN_CMD_C1_RQMEM_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class C1Rqmem : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, C1Rqmem const & cmd);

public:

    explicit C1Rqmem();
    ~C1Rqmem() = default;

    C1Rqmem(C1Rqmem && rhs) = default;
    C1Rqmem & operator=(C1Rqmem && rhs) = default;
    C1Rqmem(C1Rqmem const & rhs) = default;
    C1Rqmem & operator=(C1Rqmem const & rhs) = default;
    std::string const cmd_name = "c1_rqmem";

    CmdFieldHex<uint32_t> starting_address;
    CmdField<uint16_t> byte_count;
    BmMemoryType memory_type;
    CmdFieldArrayUByte<16> pw;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, C1Rqmem const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C1_RQMEM_H
