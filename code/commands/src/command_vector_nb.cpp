// ** CommandVectorNb Class ** //
// Manzano software

#include "command_vector_nb.h"
namespace mzn {

// -------------------------------------------------------------------------- //
CommandVectorNb::CommandVectorNb(uint16_t const cmd_number,
                             uint16_t const cmd_data_size) :
        CommandContainer(cmd_number, cmd_data_size) {}

// move constructor
// -------------------------------------------------------------------------- //
CommandVectorNb::CommandVectorNb(CommandVectorNb && rhs) noexcept :
        CommandContainer( std::move(rhs) ) {}

// -------------------------------------------------------------------------- //
uint16_t CommandVectorNb::msg_to_data(std::vector<uint8_t> const & msg,
                                    uint16_t mf_begin) {
    // called typically after msg_to_data on header cmd_field(s)

    // the first step is to find out which parts of the msg belong
    // to which command. This is particularly important because
    // some of the commands have an unknown size at compile time
    // so the the message must be parsed in order

    // in case this function gets called more than once
    inner_commands.clear();

    // unique for each command, not auto generated
    // nb : number of bytes of inner commands
    auto const N = this->nb(msg, mf_begin);

    std::cout << std::endl << "____  N:" << N;

    // loop over inner commands
    int8_t map_key = 0;

    auto const vector_begin_index = mf_begin;

    while (mf_begin < vector_begin_index + N) {

        // create new ic unique_ptr and insert to inner_commands
        // pure virtual in command_container, @throw if nullptr
        create_new_ic(map_key);
        map_key++;
        mf_begin = inner_commands.back()->msg_to_data(msg, mf_begin);
    }

    return mf_begin;
}

} // << mzn
