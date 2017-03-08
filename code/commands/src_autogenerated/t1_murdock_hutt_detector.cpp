#include "t1_murdock_hutt_detector.h"

namespace mzn {
T1MurdockHuttDetector::T1MurdockHuttDetector():
    Command(132, 25),
    number_of_bytes(),
    detector_number(),
    detector_filter_number(),
    iw_parameter(),
    nht_parameter(),
    filhi_parameter(),
    fillo_parameter(),
    wa_parameter(),
    spare_1(),
    tc_parameter(),
    x1_parameter_over_2(),
    x2_parameter_over_2(),
    x3_parameter_over_2(),
    xx_parameter(),
    av_parameter(),
    detector_name() { }

uint16_t T1MurdockHuttDetector::msg_to_data(std::vector<uint8_t> const & msg,
                                            uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1MurdockHuttDetector",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.msg_to_data(msg, mf_begin);
    mf_begin = detector_number.msg_to_data(msg, mf_begin);
    mf_begin = detector_filter_number.msg_to_data(msg, mf_begin);
    mf_begin = iw_parameter.msg_to_data(msg, mf_begin);
    mf_begin = nht_parameter.msg_to_data(msg, mf_begin);
    mf_begin = filhi_parameter.msg_to_data(msg, mf_begin);
    mf_begin = fillo_parameter.msg_to_data(msg, mf_begin);
    mf_begin = wa_parameter.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = tc_parameter.msg_to_data(msg, mf_begin);
    mf_begin = x1_parameter_over_2.msg_to_data(msg, mf_begin);
    mf_begin = x2_parameter_over_2.msg_to_data(msg, mf_begin);
    mf_begin = x3_parameter_over_2.msg_to_data(msg, mf_begin);
    mf_begin = xx_parameter.msg_to_data(msg, mf_begin);
    mf_begin = av_parameter.msg_to_data(msg, mf_begin);
    mf_begin = detector_name.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1MurdockHuttDetector::data_to_msg(std::vector<uint8_t> & msg,
                                            uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1MurdockHuttDetector",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.data_to_msg(msg, mf_begin);
    mf_begin = detector_number.data_to_msg(msg, mf_begin);
    mf_begin = detector_filter_number.data_to_msg(msg, mf_begin);
    mf_begin = iw_parameter.data_to_msg(msg, mf_begin);
    mf_begin = nht_parameter.data_to_msg(msg, mf_begin);
    mf_begin = filhi_parameter.data_to_msg(msg, mf_begin);
    mf_begin = fillo_parameter.data_to_msg(msg, mf_begin);
    mf_begin = wa_parameter.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = tc_parameter.data_to_msg(msg, mf_begin);
    mf_begin = x1_parameter_over_2.data_to_msg(msg, mf_begin);
    mf_begin = x2_parameter_over_2.data_to_msg(msg, mf_begin);
    mf_begin = x3_parameter_over_2.data_to_msg(msg, mf_begin);
    mf_begin = xx_parameter.data_to_msg(msg, mf_begin);
    mf_begin = av_parameter.data_to_msg(msg, mf_begin);
    mf_begin = detector_name.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1MurdockHuttDetector::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_MURDOCK_HUTT_DETECTOR ---  \n";

    cmd_os << "\nnumber_of_bytes: "; cmd_os << number_of_bytes;

    cmd_os << "\ndetector_number: "; cmd_os << detector_number;

    cmd_os << "\ndetector_filter_number: "; cmd_os << detector_filter_number;

    cmd_os << "\niw_parameter: "; cmd_os << iw_parameter;

    cmd_os << "\nnht_parameter: "; cmd_os << nht_parameter;

    cmd_os << "\nfilhi_parameter: "; cmd_os << filhi_parameter;

    cmd_os << "\nfillo_parameter: "; cmd_os << fillo_parameter;

    cmd_os << "\nwa_parameter: "; cmd_os << wa_parameter;

    cmd_os << "\ntc_parameter: "; cmd_os << tc_parameter;

    cmd_os << "\nx1_parameter_over_2: "; cmd_os << x1_parameter_over_2;

    cmd_os << "\nx2_parameter_over_2: "; cmd_os << x2_parameter_over_2;

    cmd_os << "\nx3_parameter_over_2: "; cmd_os << x3_parameter_over_2;

    cmd_os << "\nxx_parameter: "; cmd_os << xx_parameter;

    cmd_os << "\nav_parameter: "; cmd_os << av_parameter;

    cmd_os << "\ndetector_name: "; cmd_os << detector_name;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace