#include "t1_threshold_detector.h"

namespace mzn {
T1ThresholdDetector::T1ThresholdDetector():
    Command(133, 18),
    number_of_bytes(),
    detector_number(),
    detector_filter_number(),
    hysterisis_parameter(),
    window_parameter(),
    high_limit_parameter(),
    low_limit_parameter(),
    tail_parameter(),
    spare_1(),
    detector_name() { }

uint16_t T1ThresholdDetector::msg_to_data(std::vector<uint8_t> const & msg,
                                          uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1ThresholdDetector",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.msg_to_data(msg, mf_begin);
    mf_begin = detector_number.msg_to_data(msg, mf_begin);
    mf_begin = detector_filter_number.msg_to_data(msg, mf_begin);
    mf_begin = hysterisis_parameter.msg_to_data(msg, mf_begin);
    mf_begin = window_parameter.msg_to_data(msg, mf_begin);
    mf_begin = high_limit_parameter.msg_to_data(msg, mf_begin);
    mf_begin = low_limit_parameter.msg_to_data(msg, mf_begin);
    mf_begin = tail_parameter.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = detector_name.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1ThresholdDetector::data_to_msg(std::vector<uint8_t> & msg,
                                          uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1ThresholdDetector",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.data_to_msg(msg, mf_begin);
    mf_begin = detector_number.data_to_msg(msg, mf_begin);
    mf_begin = detector_filter_number.data_to_msg(msg, mf_begin);
    mf_begin = hysterisis_parameter.data_to_msg(msg, mf_begin);
    mf_begin = window_parameter.data_to_msg(msg, mf_begin);
    mf_begin = high_limit_parameter.data_to_msg(msg, mf_begin);
    mf_begin = low_limit_parameter.data_to_msg(msg, mf_begin);
    mf_begin = tail_parameter.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = detector_name.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1ThresholdDetector::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_THRESHOLD_DETECTOR ---  \n";

    cmd_os << "\nnumber_of_bytes: "; cmd_os << number_of_bytes;

    cmd_os << "\ndetector_number: "; cmd_os << detector_number;

    cmd_os << "\ndetector_filter_number: "; cmd_os << detector_filter_number;

    cmd_os << "\nhysterisis_parameter: "; cmd_os << hysterisis_parameter;

    cmd_os << "\nwindow_parameter: "; cmd_os << window_parameter;

    cmd_os << "\nhigh_limit_parameter: "; cmd_os << high_limit_parameter;

    cmd_os << "\nlow_limit_parameter: "; cmd_os << low_limit_parameter;

    cmd_os << "\ntail_parameter: "; cmd_os << tail_parameter;

    cmd_os << "\ndetector_name: "; cmd_os << detector_name;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace