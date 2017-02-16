#include "c1_rqmem.h"

namespace mzn {
C1Rqmem::C1Rqmem():
    Command(0x41, 24),
    starting_address(),
    byte_count(),
    memory_type(),
    pw() { }

uint16_t C1Rqmem::msg_to_data(std::vector<uint8_t> const & msg,
                              uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Rqmem",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = starting_address.msg_to_data(msg, mf_begin);
    mf_begin = byte_count.msg_to_data(msg, mf_begin);
    mf_begin = memory_type.msg_to_data(msg, mf_begin);
    mf_begin = pw.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Rqmem::data_to_msg(std::vector<uint8_t> & msg,
                              uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Rqmem",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = starting_address.data_to_msg(msg, mf_begin);
    mf_begin = byte_count.data_to_msg(msg, mf_begin);
    mf_begin = memory_type.data_to_msg(msg, mf_begin);
    mf_begin = pw.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Rqmem::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_RQMEM ---  \n";

    cmd_os << "\nstarting_address: "; cmd_os << starting_address;

    cmd_os << "\nbyte_count: "; cmd_os << byte_count;

    cmd_os << "\nmemory_type: "; cmd_os << memory_type;

    cmd_os << "\npw: "; cmd_os << pw;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace