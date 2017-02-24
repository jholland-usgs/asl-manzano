// ** T1DataServer Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_T1_DATA_SERVER_H
#define _MZN_CMD_T1_DATA_SERVER_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class T1DataServer : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, T1DataServer const & cmd);

public:

    explicit T1DataServer();
    ~T1DataServer() = default;

    T1DataServer(T1DataServer && rhs) = default;
    T1DataServer & operator=(T1DataServer && rhs) = default;
    T1DataServer(T1DataServer const & rhs) = default;
    T1DataServer & operator=(T1DataServer const & rhs) = default;
    std::string const cmd_name = "t1_data_server";

    CmdField<uint16_t> port_number;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, T1DataServer const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_T1_DATA_SERVER_H
