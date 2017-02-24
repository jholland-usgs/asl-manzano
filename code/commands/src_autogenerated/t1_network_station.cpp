#include "t1_network_station.h"

namespace mzn {
T1NetworkStation::T1NetworkStation():
    Command(2, 5),
    network(),
    station() { }

uint16_t T1NetworkStation::msg_to_data(std::vector<uint8_t> const & msg,
                                       uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1NetworkStation",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = network.msg_to_data(msg, mf_begin);
    mf_begin = station.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1NetworkStation::data_to_msg(std::vector<uint8_t> & msg,
                                       uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1NetworkStation",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = network.data_to_msg(msg, mf_begin);
    mf_begin = station.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1NetworkStation::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_NETWORK_STATION ---  \n";

    cmd_os << "\nnetwork: "; cmd_os << network;

    cmd_os << "\nstation: "; cmd_os << station;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace