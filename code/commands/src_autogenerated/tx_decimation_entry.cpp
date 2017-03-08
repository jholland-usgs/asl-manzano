#include "tx_decimation_entry.h"

namespace mzn {
TxDecimationEntry::TxDecimationEntry():
    Command(0, 2),
    source_lcq_number(),
    decimation_fir_filter_number() { }

uint16_t TxDecimationEntry::msg_to_data(std::vector<uint8_t> const & msg,
                                        uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxDecimationEntry",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = source_lcq_number.msg_to_data(msg, mf_begin);
    mf_begin = decimation_fir_filter_number.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxDecimationEntry::data_to_msg(std::vector<uint8_t> & msg,
                                        uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxDecimationEntry",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = source_lcq_number.data_to_msg(msg, mf_begin);
    mf_begin = decimation_fir_filter_number.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxDecimationEntry::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_DECIMATION_ENTRY ---  \n";

    cmd_os << "\nsource_lcq_number: "; cmd_os << source_lcq_number;

    cmd_os << "\ndecimation_fir_filter_number: "; cmd_os << decimation_fir_filter_number;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace