#include "t1_log_and_timing.h"

namespace mzn {
T1LogAndTiming::T1LogAndTiming():
    Command(7, 10),
    message_log_seed_location(),
    message_log_seed_name(),
    timing_log_seed_location(),
    timing_log_seed_name() { }

uint16_t T1LogAndTiming::msg_to_data(std::vector<uint8_t> const & msg,
                                     uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1LogAndTiming",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = message_log_seed_location.msg_to_data(msg, mf_begin);
    mf_begin = message_log_seed_name.msg_to_data(msg, mf_begin);
    mf_begin = timing_log_seed_location.msg_to_data(msg, mf_begin);
    mf_begin = timing_log_seed_name.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1LogAndTiming::data_to_msg(std::vector<uint8_t> & msg,
                                     uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1LogAndTiming",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = message_log_seed_location.data_to_msg(msg, mf_begin);
    mf_begin = message_log_seed_name.data_to_msg(msg, mf_begin);
    mf_begin = timing_log_seed_location.data_to_msg(msg, mf_begin);
    mf_begin = timing_log_seed_name.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1LogAndTiming::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_LOG_AND_TIMING ---  \n";

    cmd_os << "\nmessage_log_seed_location: "; cmd_os << message_log_seed_location;

    cmd_os << "\nmessage_log_seed_name: "; cmd_os << message_log_seed_name;

    cmd_os << "\ntiming_log_seed_location: "; cmd_os << timing_log_seed_location;

    cmd_os << "\ntiming_log_seed_name: "; cmd_os << timing_log_seed_name;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace