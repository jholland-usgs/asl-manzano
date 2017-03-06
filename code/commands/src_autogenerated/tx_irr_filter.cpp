#include "tx_irr_filter.h"

namespace mzn {
TxIrrFilter::TxIrrFilter():
    Command(0, 5),
    cutoff_frequency_ratio(),
    number_of_poles() { }

uint16_t TxIrrFilter::msg_to_data(std::vector<uint8_t> const & msg,
                                  uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxIrrFilter",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = cutoff_frequency_ratio.msg_to_data(msg, mf_begin);
    mf_begin = number_of_poles.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxIrrFilter::data_to_msg(std::vector<uint8_t> & msg,
                                  uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxIrrFilter",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = cutoff_frequency_ratio.data_to_msg(msg, mf_begin);
    mf_begin = number_of_poles.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxIrrFilter::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_IRR_FILTER ---  \n";

    cmd_os << "\ncutoff_frequency_ratio: "; cmd_os << cutoff_frequency_ratio;

    cmd_os << "\nnumber_of_poles: "; cmd_os << number_of_poles;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace