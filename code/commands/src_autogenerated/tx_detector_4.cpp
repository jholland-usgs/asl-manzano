#include "tx_detector_4.h"

namespace mzn {
TxDetector4::TxDetector4():
    Command(0, 3),
    detector_number_to_use_as_base(),
    detector_number_of_this_invocation(),
    detector_options() { }

uint16_t TxDetector4::msg_to_data(std::vector<uint8_t> const & msg,
                                  uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxDetector4",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = detector_number_to_use_as_base.msg_to_data(msg, mf_begin);
    mf_begin = detector_number_of_this_invocation.msg_to_data(msg, mf_begin);
    mf_begin = detector_options.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxDetector4::data_to_msg(std::vector<uint8_t> & msg,
                                  uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxDetector4",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = detector_number_to_use_as_base.data_to_msg(msg, mf_begin);
    mf_begin = detector_number_of_this_invocation.data_to_msg(msg, mf_begin);
    mf_begin = detector_options.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxDetector4::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_DETECTOR_4 ---  \n";

    cmd_os << "\ndetector_number_to_use_as_base: "; cmd_os << detector_number_to_use_as_base;

    cmd_os << "\ndetector_number_of_this_invocation: "; cmd_os << detector_number_of_this_invocation;

    cmd_os << "\ndetector_options: "; cmd_os << detector_options;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace