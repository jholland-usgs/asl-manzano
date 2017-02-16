#include "c1_mem.h"

namespace mzn {
C1Mem::C1Mem():
    Command(0xB8, 484),
    mem() { }

uint16_t C1Mem::msg_to_data(std::vector<uint8_t> const & msg,
                            uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Mem",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = mem.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Mem::data_to_msg(std::vector<uint8_t> & msg,
                            uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Mem",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = mem.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Mem::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_MEM ---  \n";

    cmd_os << "\nmem: "; cmd_os << mem;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace