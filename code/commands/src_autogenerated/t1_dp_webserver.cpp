#include "t1_dp_webserver.h"

namespace mzn {
T1DpWebserver::T1DpWebserver():
    Command(5, 2),
    port_number() { }

uint16_t T1DpWebserver::msg_to_data(std::vector<uint8_t> const & msg,
                                    uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1DpWebserver",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = port_number.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1DpWebserver::data_to_msg(std::vector<uint8_t> & msg,
                                    uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1DpWebserver",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = port_number.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1DpWebserver::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_DP_WEBSERVER ---  \n";

    cmd_os << "\nport_number: "; cmd_os << port_number;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace