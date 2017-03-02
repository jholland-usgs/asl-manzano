// ** T1CommEventName Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_T1_COMM_EVENT_NAME_H
#define _MZN_CMD_T1_COMM_EVENT_NAME_H

#include "command.h"


#include "tx_comm_event_name.h"

#define k_tx_comm_event_name 0

#include "multi_command.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class T1CommEventName : public MultiCommand {

friend std::ostream & operator<<(std::ostream & cmd_os, T1CommEventName const & cmd);

public:

    explicit T1CommEventName();
    ~T1CommEventName() = default;

    T1CommEventName(T1CommEventName && rhs) = default;
    T1CommEventName & operator=(T1CommEventName && rhs) = default;
    std::string const cmd_name = "t1_comm_event_name";

    CmdField<uint16_t> number_of_bytes;

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


inline std::ostream & operator<<(std::ostream & cmd_os, T1CommEventName const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_T1_COMM_EVENT_NAME_H
