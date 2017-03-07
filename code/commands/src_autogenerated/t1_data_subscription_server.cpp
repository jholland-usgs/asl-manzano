#include "t1_data_subscription_server.h"

namespace mzn {
T1DataSubscriptionServer::T1DataSubscriptionServer():
    Command(4, 40),
    highest_priority_password(),
    middle_priority_password(),
    lowest_priority_password(),
    timeout(),
    max_bytes_per_second(),
    verbosity(),
    max_cpu_percentage(),
    udp_port_number(),
    max_memory_to_use("kb"),
    ignored_0() { }

uint16_t T1DataSubscriptionServer::msg_to_data(std::vector<uint8_t> const & msg,
                                               uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1DataSubscriptionServer",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = highest_priority_password.msg_to_data(msg, mf_begin);
    mf_begin = middle_priority_password.msg_to_data(msg, mf_begin);
    mf_begin = lowest_priority_password.msg_to_data(msg, mf_begin);
    mf_begin = timeout.msg_to_data(msg, mf_begin);
    mf_begin = max_bytes_per_second.msg_to_data(msg, mf_begin);
    mf_begin = verbosity.msg_to_data(msg, mf_begin);
    mf_begin = max_cpu_percentage.msg_to_data(msg, mf_begin);
    mf_begin = udp_port_number.msg_to_data(msg, mf_begin);
    mf_begin = max_memory_to_use.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type

    return mf_begin;
}

uint16_t T1DataSubscriptionServer::data_to_msg(std::vector<uint8_t> & msg,
                                               uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1DataSubscriptionServer",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = highest_priority_password.data_to_msg(msg, mf_begin);
    mf_begin = middle_priority_password.data_to_msg(msg, mf_begin);
    mf_begin = lowest_priority_password.data_to_msg(msg, mf_begin);
    mf_begin = timeout.data_to_msg(msg, mf_begin);
    mf_begin = max_bytes_per_second.data_to_msg(msg, mf_begin);
    mf_begin = verbosity.data_to_msg(msg, mf_begin);
    mf_begin = max_cpu_percentage.data_to_msg(msg, mf_begin);
    mf_begin = udp_port_number.data_to_msg(msg, mf_begin);
    mf_begin = max_memory_to_use.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type

    return mf_begin;
}

std::ostream & T1DataSubscriptionServer::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_DATA_SUBSCRIPTION_SERVER ---  \n";

    cmd_os << "\nhighest_priority_password: "; cmd_os << highest_priority_password;

    cmd_os << "\nmiddle_priority_password: "; cmd_os << middle_priority_password;

    cmd_os << "\nlowest_priority_password: "; cmd_os << lowest_priority_password;

    cmd_os << "\ntimeout: "; cmd_os << timeout;

    cmd_os << "\nmax_bytes_per_second: "; cmd_os << max_bytes_per_second;

    cmd_os << "\nverbosity: "; cmd_os << verbosity;

    cmd_os << "\nmax_cpu_percentage: "; cmd_os << max_cpu_percentage;

    cmd_os << "\nudp_port_number: "; cmd_os << udp_port_number;

    cmd_os << "\nmax_memory_to_use: "; cmd_os << max_memory_to_use;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace