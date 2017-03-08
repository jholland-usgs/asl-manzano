#include "tx_fir_multiplier.h"

namespace mzn {
TxFirMultiplier::TxFirMultiplier():
    Command(0, 4),
    fir_multiplier() { }

uint16_t TxFirMultiplier::msg_to_data(std::vector<uint8_t> const & msg,
                                      uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxFirMultiplier",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = fir_multiplier.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxFirMultiplier::data_to_msg(std::vector<uint8_t> & msg,
                                      uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxFirMultiplier",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = fir_multiplier.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxFirMultiplier::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_FIR_MULTIPLIER ---  \n";

    cmd_os << "\nfir_multiplier: "; cmd_os << fir_multiplier;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace