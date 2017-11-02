// ** C1Mod Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C1_MOD_H
#define _MZN_CMD_C1_MOD_H

#include "command.h"


#include "cx_mod.h"

#include "command_vector_ni.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class C1Mod : public CommandVectorNi {

friend std::ostream & operator<<(std::ostream & cmd_os, C1Mod const & cmd);

public:

    explicit C1Mod();
    ~C1Mod() = default;

    C1Mod(C1Mod && rhs) = default;
    C1Mod & operator=(C1Mod && rhs) = default;
    std::string const cmd_name = "c1_mod";


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


inline std::ostream & operator<<(std::ostream & cmd_os, C1Mod const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C1_MOD_H
