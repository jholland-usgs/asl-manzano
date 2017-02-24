// ** T1DataSubscriptionServerParameter Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_T1_DATA_SUBSCRIPTION_SERVER_PARAMETER_H
#define _MZN_CMD_T1_DATA_SUBSCRIPTION_SERVER_PARAMETER_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class T1DataSubscriptionServerParameter : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, T1DataSubscriptionServerParameter const & cmd);

public:

    explicit T1DataSubscriptionServerParameter();
    ~T1DataSubscriptionServerParameter() = default;

    T1DataSubscriptionServerParameter(T1DataSubscriptionServerParameter && rhs) = default;
    T1DataSubscriptionServerParameter & operator=(T1DataSubscriptionServerParameter && rhs) = default;
    T1DataSubscriptionServerParameter(T1DataSubscriptionServerParameter const & rhs) = default;
    T1DataSubscriptionServerParameter & operator=(T1DataSubscriptionServerParameter const & rhs) = default;
    std::string const cmd_name = "t1_data_subscription_server_parameter";

    CmdFieldPstring<8> highest_priority_password;
    CmdFieldPstring<8> middle_priority_password;
    CmdFieldPstring<8> lowest_priority_password;
    CmdFieldDuration<int32_t> timeout;
    CmdField<int32_t> max_bytes_per_second;
    CmdField<uint8_t> verbosity;
    CmdField<int8_t> max_cpu_percentage;
    CmdField<uint16_t> udp_port_number;
    CmdField<uint16_t> max_memory_to_use;
    CmdFieldIgnore<uint16_t> ignored_0;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, T1DataSubscriptionServerParameter const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_T1_DATA_SUBSCRIPTION_SERVER_PARAMETER_H
