#include "tx_comm_event_name.h"

namespace mzn {
TxCommEventName::TxCommEventName():
    Command(0, 1),
    event_number(),
    event_name() { }

uint16_t TxCommEventName::msg_to_data(std::vector<uint8_t> const & msg,
                                      uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxCommEventName",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    std::cout << std::endl << "###" << mf_begin << "###\n";
    mf_begin = event_number.msg_to_data(msg, mf_begin);

    std::cout << std::endl << "___" << event_number << "___\n";
    std::cout << std::endl << "###" << mf_begin << "###\n";
    std::cout << std::endl << "before ___" << event_name << "___\n";

    mf_begin = event_name.msg_to_data(msg, mf_begin);

    std::cout << std::endl << "###" << mf_begin << "###\n";
    std::cout << std::endl << "after ___" << event_name << "___\n";
    return mf_begin;
}

uint16_t TxCommEventName::data_to_msg(std::vector<uint8_t> & msg,
                                      uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxCommEventName",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = event_number.data_to_msg(msg, mf_begin);
    mf_begin = event_name.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxCommEventName::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_COMM_EVENT_NAME ---  \n";

    cmd_os << "\nevent_number: "; cmd_os << event_number;

    cmd_os << "\nevent_name: "; cmd_os << event_name;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace
