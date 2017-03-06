#include "cx_arp_status.h"

namespace mzn {
CxArpStatus::CxArpStatus():
    CommandContainer(0, 4),
    number_of_entries(),
    size_of_this_block() { }

uint16_t CxArpStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                  uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxArpStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_entries.msg_to_data(msg, mf_begin);
    mf_begin = size_of_this_block.msg_to_data(msg, mf_begin);
    mf_begin = CommandContainer::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxArpStatus::data_to_msg(std::vector<uint8_t> & msg,
                                  uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxArpStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_entries.data_to_msg(msg, mf_begin);
    mf_begin = size_of_this_block.data_to_msg(msg, mf_begin);
    mf_begin = CommandContainer::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxArpStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_ARP_STATUS ---  \n";

    cmd_os << "\nnumber_of_entries: "; cmd_os << number_of_entries;

    cmd_os << "\nsize_of_this_block: "; cmd_os << size_of_this_block;
    cmd_os << std::endl;

    return CommandContainer::os_print(cmd_os);
}


void CxArpStatus::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<CyArpStatus>() } );

    if (inner_commands.back() == nullptr) {
        throw WarningException("CyArpStatus",
                               "create_new_ic",
                               "nullptr inner command");
     }

}
} // end namespace