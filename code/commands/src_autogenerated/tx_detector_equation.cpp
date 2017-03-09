#include "tx_detector_equation.h"

namespace mzn {
TxDetectorEquation::TxDetectorEquation():
    Command(0, 1),
    element() { }

uint16_t TxDetectorEquation::msg_to_data(std::vector<uint8_t> const & msg,
                                         uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxDetectorEquation",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = element.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxDetectorEquation::data_to_msg(std::vector<uint8_t> & msg,
                                         uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxDetectorEquation",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = element.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxDetectorEquation::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_DETECTOR_EQUATION ---  \n";

    cmd_os << "\nelement: "; cmd_os << element;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace