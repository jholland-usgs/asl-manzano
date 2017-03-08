#include "tx_pre_event_buffers.h"

namespace mzn {
TxPreEventBuffers::TxPreEventBuffers():
    Command(0, 2),
    number_of_pre_event_buffers() { }

uint16_t TxPreEventBuffers::msg_to_data(std::vector<uint8_t> const & msg,
                                        uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxPreEventBuffers",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_pre_event_buffers.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxPreEventBuffers::data_to_msg(std::vector<uint8_t> & msg,
                                        uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxPreEventBuffers",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_pre_event_buffers.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxPreEventBuffers::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_PRE_EVENT_BUFFERS ---  \n";

    cmd_os << "\nnumber_of_pre_event_buffers: "; cmd_os << number_of_pre_event_buffers;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace