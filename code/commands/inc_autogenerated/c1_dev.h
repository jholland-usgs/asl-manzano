// ** C1Dev Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C1_DEV_H
#define _MZN_CMD_C1_DEV_H

#include "command.h"


#include "cx_dev.h"

namespace mzn {

// -------------------------------------------------------------------------- //


enum class C1DevKey {
    cx_dev = 0,
};
} // <- mzn
#include "multi_command.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class C1Dev : public MultiCommand {

friend std::ostream & operator<<(std::ostream & cmd_os, C1Dev const & cmd);

public:

    explicit C1Dev();
    ~C1Dev() = default;

    C1Dev(C1Dev && rhs) = default;
    C1Dev & operator=(C1Dev && rhs) = default;
    std::string const cmd_name = "c1_dev";


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


inline std::ostream & operator<<(std::ostream & cmd_os, C1Dev const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C1_DEV_H
