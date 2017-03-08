#include "tx_gap_threshold.h"

namespace mzn {
TxGapThreshold::TxGapThreshold():
    Command(0, 4),
    gap_threshold() { }

uint16_t TxGapThreshold::msg_to_data(std::vector<uint8_t> const & msg,
                                     uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxGapThreshold",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = gap_threshold.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxGapThreshold::data_to_msg(std::vector<uint8_t> & msg,
                                     uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxGapThreshold",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = gap_threshold.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxGapThreshold::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_GAP_THRESHOLD ---  \n";

    cmd_os << "\ngap_threshold: "; cmd_os << gap_threshold;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace