// ** CommandMapNi Class ** //
// Manzano software

#include "command_map_ni.h"
namespace mzn {

// -------------------------------------------------------------------------- //
CommandMapNi::CommandMapNi(uint16_t const cmd_number,
                                 uint16_t const cmd_data_size) :
        MultiCommand(cmd_number, cmd_data_size) {}

// move constructor
// -------------------------------------------------------------------------- //
CommandMapNi::CommandMapNi(CommandMapNi && rhs) noexcept :
        MultiCommand( std::move(rhs) ),
        command_index_map_( std::move(rhs.command_index_map_) ) {}

/*
// move assignment operator
// -------------------------------------------------------------------------- //
CommandMapNi & CommandMapNi::operator=(CommandMapNi && rhs) noexcept {
    // avoid self-assignment
    if (this != &rhs) {
        MultiCommand::operator=( std::move(rhs) );
        command_index_map_ = std::move(rhs.command_index_map_);
    }
    return *this;
}
*/
// -------------------------------------------------------------------------- //
void CommandMapNi::set_command_index(uint8_t const map_key,
                                        int const cmd_index) {
    command_index_map_.emplace(map_key, cmd_index);
}

// -------------------------------------------------------------------------- //
int CommandMapNi::command_index(uint8_t const map_key) const {

    // map does not allow for duplicates
    if (command_index_map_.count(map_key) == 1) {
        // using .at() due that operator[] is not const
        return command_index_map_.at(map_key);
    } else {
        throw FatalException("CommandMapNi",
                             "command_index",
                             "bad multi command format, \
                             \nmulti command maps dont allow for duplicates");
    }
}

// -------------------------------------------------------------------------- //
uint16_t CommandMapNi::msg_to_data(std::vector<uint8_t> const & msg,
                                      uint16_t mf_begin) {


    // called typically after msg_to_data on header cmd_field(s)

    // the first step is to find out which parts of the msg belong
    // to which command. This is particularly important because
    // some of the commands have an unknown size at compile time
    // so the the message must be parsed in order

    // in case this function gets called more than once
    inner_commands.clear();

    // unique for each command, not auto generated
    // nic : number of inner commands
    auto const nic = number_of_ic(msg, mf_begin);

    // loop over inner commands
    for (auto map_key = 0; map_key < nic; map_key++) {

        if ( command_active(map_key) ) {

            // create new ic unique_ptr and insert to inner_commands
            // pure virtual in multi_command, @throw if nullptr
            create_new_ic(map_key);

            mf_begin = inner_commands.back() -> msg_to_data(msg, mf_begin);

            auto const cmd_index = inner_commands.size();

            set_command_index(map_key, cmd_index );
        }
    }
    return mf_begin;
}

// -------------------------------------------------------------------------- //
uint16_t CommandMapNi::data_to_msg(std::vector<uint8_t> & msg,
                                      uint16_t mf_begin) const {

    for (auto const & ic : inner_commands) {
        mf_begin = ic -> data_to_msg(msg, mf_begin);
    }
    return mf_begin ;
}

} // << mzn
