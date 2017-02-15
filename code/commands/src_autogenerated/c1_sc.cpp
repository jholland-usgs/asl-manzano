#include "c1_sc.h"

namespace mzn {
C1Sc::C1Sc():
    Command(0xAF, 32),
    sensor_output_1a_ignore(),
    sensor_output_1a_active_high(),
    sensor_output_1a(),
    sensor_output_2a_ignore(),
    sensor_output_2a_active_high(),
    sensor_output_2a(),
    sensor_output_3a_ignore(),
    sensor_output_3a_active_high(),
    sensor_output_3a(),
    sensor_output_4a_ignore(),
    sensor_output_4a_active_high(),
    sensor_output_4a(),
    sensor_output_1b_ignore(),
    sensor_output_1b_active_high(),
    sensor_output_1b(),
    sensor_output_2b_ignore(),
    sensor_output_2b_active_high(),
    sensor_output_2b(),
    sensor_output_3b_ignore(),
    sensor_output_3b_active_high(),
    sensor_output_3b(),
    sensor_output_4b_ignore(),
    sensor_output_4b_active_high(),
    sensor_output_4b() { }

uint16_t C1Sc::msg_to_data(std::vector<uint8_t> const & msg,
                           uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Sc",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_1a_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_1a.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_2a_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_2a.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_3a_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_3a.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_4a_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_4a.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_1b_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_1b.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_2b_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_2b.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_3b_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_3b.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_4b_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_4b.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Sc::data_to_msg(std::vector<uint8_t> & msg,
                           uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Sc",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_1a_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_1a.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_2a_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_2a.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_3a_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_3a.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_4a_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_4a.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_1b_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_1b.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_2b_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_2b.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_3b_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_3b.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_4b_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_4b.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Sc::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_SC ---  \n";

    cmd_os << "\nsensor_output_1a_active_high: "; cmd_os << sensor_output_1a_active_high;

    cmd_os << "\nsensor_output_1a: "; cmd_os << sensor_output_1a;

    cmd_os << "\nsensor_output_2a_active_high: "; cmd_os << sensor_output_2a_active_high;

    cmd_os << "\nsensor_output_2a: "; cmd_os << sensor_output_2a;

    cmd_os << "\nsensor_output_3a_active_high: "; cmd_os << sensor_output_3a_active_high;

    cmd_os << "\nsensor_output_3a: "; cmd_os << sensor_output_3a;

    cmd_os << "\nsensor_output_4a_active_high: "; cmd_os << sensor_output_4a_active_high;

    cmd_os << "\nsensor_output_4a: "; cmd_os << sensor_output_4a;

    cmd_os << "\nsensor_output_1b_active_high: "; cmd_os << sensor_output_1b_active_high;

    cmd_os << "\nsensor_output_1b: "; cmd_os << sensor_output_1b;

    cmd_os << "\nsensor_output_2b_active_high: "; cmd_os << sensor_output_2b_active_high;

    cmd_os << "\nsensor_output_2b: "; cmd_os << sensor_output_2b;

    cmd_os << "\nsensor_output_3b_active_high: "; cmd_os << sensor_output_3b_active_high;

    cmd_os << "\nsensor_output_3b: "; cmd_os << sensor_output_3b;

    cmd_os << "\nsensor_output_4b_active_high: "; cmd_os << sensor_output_4b_active_high;

    cmd_os << "\nsensor_output_4b: "; cmd_os << sensor_output_4b;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace