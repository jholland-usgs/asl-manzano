#include "tx_maximum_frame_count.h"

namespace mzn {
TxMaximumFrameCount::TxMaximumFrameCount():
    Command(0, 1),
    maximum_frame_count() { }

uint16_t TxMaximumFrameCount::msg_to_data(std::vector<uint8_t> const & msg,
                                          uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxMaximumFrameCount",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = maximum_frame_count.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxMaximumFrameCount::data_to_msg(std::vector<uint8_t> & msg,
                                          uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxMaximumFrameCount",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = maximum_frame_count.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxMaximumFrameCount::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_MAXIMUM_FRAME_COUNT ---  \n";

    cmd_os << "\nmaximum_frame_count: "; cmd_os << maximum_frame_count;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace