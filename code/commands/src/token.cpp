// ** Token Class ** //
// Manzano software

#include "token.h"

namespace mzn {

// -------------------------------------------------------------------------- //
Token::Token(uint16_t const cmd_number,
             uint16_t const cmd_data_size) :
        CommandContainer(cmd_number, cmd_data_size) {}

// move constructor
// -------------------------------------------------------------------------- //
Token::Token(Token && rhs) noexcept :
        CommandContainer( std::move(rhs) ) {}

// -------------------------------------------------------------------------- //
uint16_t Token::msg_to_data(std::vector<uint8_t> const & msg,
                                      uint16_t mf_begin) {
    // called typically after msg_to_data on header cmd_field(s)

    // the first step is to find out which parts of the msg belong
    // to which command. This is particularly important because
    // some of the commands have an unknown size at compile time
    // so the the message must be parsed in order

    // in case this function gets called more than once
    inner_commands.clear();

    /*
    // unique for each command, not auto generated
    // nic : number of inner commands
    auto const nic = number_of_ic(msg, mf_begin);

    // loop over inner commands
    for (auto map_key = 0; map_key < nic; map_key++) {

        if ( command_active(map_key) ) {

            // create new ic unique_ptr and insert to inner_commands
            // pure virtual in command_container, @throw if nullptr
            create_new_ic(map_key);

            mf_begin = inner_commands.back() -> msg_to_data(msg, mf_begin);

            auto const cmd_index = inner_commands.size();

            set_command_index(map_key, cmd_index );
        }
    }
    */
    return mf_begin;
}

// -------------------------------------------------------------------------- //
uint16_t Token::data_to_msg(std::vector<uint8_t> & msg,
                                      uint16_t mf_begin) const {

    for (auto const & ic : inner_commands) {
        mf_begin = ic -> data_to_msg(msg, mf_begin);
    }
    return mf_begin ;
}

} // << mzn
