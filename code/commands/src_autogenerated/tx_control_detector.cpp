#include "tx_control_detector.h"

namespace mzn {
TxControlDetector::TxControlDetector():
    Command(0, 1),
    control_detector_number() { }

uint16_t TxControlDetector::msg_to_data(std::vector<uint8_t> const & msg,
                                        uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxControlDetector",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = control_detector_number.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxControlDetector::data_to_msg(std::vector<uint8_t> & msg,
                                        uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxControlDetector",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = control_detector_number.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxControlDetector::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_CONTROL_DETECTOR ---  \n";

    cmd_os << "\ncontrol_detector_number: "; cmd_os << control_detector_number;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace