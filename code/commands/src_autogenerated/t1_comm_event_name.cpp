#include "t1_comm_event_name.h"

namespace mzn {
T1CommEventName::T1CommEventName():
    CommandVectorNb(0, 2),
    number_of_bytes() { }

uint16_t T1CommEventName::msg_to_data(std::vector<uint8_t> const & msg,
                                      uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1CommEventName",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.msg_to_data(msg, mf_begin);
    mf_begin = CommandVectorNb::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1CommEventName::data_to_msg(std::vector<uint8_t> & msg,
                                      uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1CommEventName",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.data_to_msg(msg, mf_begin);
    mf_begin = CommandContainer::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1CommEventName::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_COMM_EVENT_NAME ---  \n";

    cmd_os << "\nnumber_of_bytes: "; cmd_os << number_of_bytes;
    cmd_os << std::endl;

    return CommandContainer::os_print(cmd_os);
}


void T1CommEventName::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<TxCommEventName>() } );

    if (inner_commands.back() == nullptr) {
        throw WarningException("TxCommEventName",
                               "create_new_ic",
                               "nullptr inner command");
     }

}
} // end namespace