#include "tx_fir_filter.h"

namespace mzn {
TxFirFilter::TxFirFilter():
    Command(0, 2),
    filter_number(),
    filter_name() { }

uint16_t TxFirFilter::msg_to_data(std::vector<uint8_t> const & msg,
                                  uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxFirFilter",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = filter_number.msg_to_data(msg, mf_begin);
    mf_begin = filter_name.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxFirFilter::data_to_msg(std::vector<uint8_t> & msg,
                                  uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxFirFilter",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = filter_number.data_to_msg(msg, mf_begin);
    mf_begin = filter_name.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxFirFilter::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_FIR_FILTER ---  \n";

    cmd_os << "\nfilter_number: "; cmd_os << filter_number;

    cmd_os << "\nfilter_name: "; cmd_os << filter_name;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace