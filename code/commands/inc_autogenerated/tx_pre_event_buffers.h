// ** TxPreEventBuffers Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_TX_PRE_EVENT_BUFFERS_H
#define _MZN_CMD_TX_PRE_EVENT_BUFFERS_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class TxPreEventBuffers : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, TxPreEventBuffers const & cmd);

public:

    explicit TxPreEventBuffers();
    ~TxPreEventBuffers() = default;

    TxPreEventBuffers(TxPreEventBuffers && rhs) = default;
    TxPreEventBuffers & operator=(TxPreEventBuffers && rhs) = default;
    TxPreEventBuffers(TxPreEventBuffers const & rhs) = default;
    TxPreEventBuffers & operator=(TxPreEventBuffers const & rhs) = default;
    std::string const cmd_name = "tx_pre_event_buffers";

    CmdField<uint16_t> number_of_pre_event_buffers;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, TxPreEventBuffers const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_TX_PRE_EVENT_BUFFERS_H
