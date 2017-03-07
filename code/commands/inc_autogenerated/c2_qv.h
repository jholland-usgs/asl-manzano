// ** C2Qv Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C2_QV_H
#define _MZN_CMD_C2_QV_H

#include "command.h"


#include "cx_qv.h"

#include "command_vector_ni.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class C2Qv : public CommandVectorNi {

friend std::ostream & operator<<(std::ostream & cmd_os, C2Qv const & cmd);

public:

    explicit C2Qv();
    ~C2Qv() = default;

    C2Qv(C2Qv && rhs) = default;
    C2Qv & operator=(C2Qv && rhs) = default;
    std::string const cmd_name = "c2_qv";

    CmdField<uint32_t> starting_sequence_number;
    CmdFieldDuration<uint16_t> seconds_count;
    CmdField<uint16_t> number_of_entries;
    BmChannelMap actual_channel_mask;
    CmdFieldIgnore<uint16_t> spare_0;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;
    // max keys implementation in a separate function/file
    // not part of auto generation
    uint16_t ni(std::vector<uint8_t> const & msg,
                uint16_t mf_begin) const override;
    void create_new_ic(uint8_t const  cmd_key) override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, C2Qv const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C2_QV_H
