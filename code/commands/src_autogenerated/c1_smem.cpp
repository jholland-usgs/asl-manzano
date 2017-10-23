#include "c1_smem.h"

namespace mzn {
C1Smem::C1Smem():
    CommandVectorNb(0x40, 8),
    starting_address(),
    byte_count(),
    memory_type() { }

uint16_t C1Smem::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Smem",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = starting_address.msg_to_data(msg, mf_begin);
    mf_begin = byte_count.msg_to_data(msg, mf_begin);
    mf_begin = memory_type.msg_to_data(msg, mf_begin);
    mf_begin = CommandVectorNb::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Smem::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Smem",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = starting_address.data_to_msg(msg, mf_begin);
    mf_begin = byte_count.data_to_msg(msg, mf_begin);
    mf_begin = memory_type.data_to_msg(msg, mf_begin);
    mf_begin = CommandContainer::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Smem::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_SMEM ---  \n";

    cmd_os << "\nstarting_address: "; cmd_os << starting_address;

    cmd_os << "\nbyte_count: "; cmd_os << byte_count;

    cmd_os << "\nmemory_type: "; cmd_os << memory_type;
    cmd_os << std::endl;

    return CommandContainer::os_print(cmd_os);
}


void C1Smem::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<CxMem>() } );

    if (inner_commands.back() == nullptr) {
        throw WarningException("CxMem",
                               "create_new_ic",
                               "nullptr inner command");
     }

}
} // end namespace