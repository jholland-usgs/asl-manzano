// ** CommandContainer Class ** //
// Manzano software

#include "command_container.h"
namespace mzn {

// normal constructor, multi_cmd_ = true
// -------------------------------------------------------------------------- //
CommandContainer::CommandContainer(uint16_t const cmd_number,
                           uint16_t const cmd_data_size) :
        Command(cmd_number, cmd_data_size, true),
        inner_commands{} {}

// move constructor
// -------------------------------------------------------------------------- //
CommandContainer::CommandContainer(CommandContainer && rhs) noexcept :
        Command( std::move(rhs) ),
        inner_commands( std::move(rhs.inner_commands) ) {}

// -------------------------------------------------------------------------- //
uint16_t CommandContainer::data_to_msg(std::vector<uint8_t> & msg,
                                   uint16_t mf_begin) const {

    for (auto const & ic : inner_commands) {
        mf_begin = ic -> data_to_msg(msg, mf_begin);
    }

    return mf_begin ;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & CommandContainer::os_print(std::ostream & cmd_os) const {

    for (auto const & ic : inner_commands) {
        cmd_os << *(ic);
        cmd_os << "\n";
    }

    return cmd_os;
}

} // << mzn
