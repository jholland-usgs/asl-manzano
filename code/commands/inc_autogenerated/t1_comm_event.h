// ** T1CommEvent Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_T1_COMM_EVENT_H
#define _MZN_CMD_T1_COMM_EVENT_H

#include "command.h"


#include "tx_comm_event.h"

#include "command_vector_nb.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class T1CommEvent : public CommandVectorNb {

friend std::ostream & operator<<(std::ostream & cmd_os, T1CommEvent const & cmd);

public:

    explicit T1CommEvent();
    ~T1CommEvent() = default;

    T1CommEvent(T1CommEvent && rhs) = default;
    T1CommEvent & operator=(T1CommEvent && rhs) = default;
    std::string const cmd_name = "t1_comm_event";

    CmdField<uint16_t> number_of_bytes;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;
    // max keys implementation in a separate function/file
    // not part of auto generation
    uint16_t nb(std::vector<uint8_t> const & msg,
                uint16_t mf_begin) const override;
    void create_new_ic(uint8_t const  cmd_key) override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, T1CommEvent const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_T1_COMM_EVENT_H
