// ** CommandMap Class ** //
// Manzano software

#include "command_map.h"
namespace mzn {

// -------------------------------------------------------------------------- //
CommandMap::CommandMap(uint16_t const cmd_number,
                                 uint16_t const cmd_data_size) :
        CommandContainer(cmd_number, cmd_data_size) {}

// move constructor
// -------------------------------------------------------------------------- //
CommandMap::CommandMap(CommandMap && rhs) noexcept :
        CommandContainer( std::move(rhs) ),
        command_index_map_( std::move(rhs.command_index_map_) ) {}

// -------------------------------------------------------------------------- //
void CommandMap::set_command_index(uint8_t const map_key,
                                        int const cmd_index) {
    command_index_map_.emplace(map_key, cmd_index);
}

// -------------------------------------------------------------------------- //
int CommandMap::command_index(uint8_t const map_key) const {

    // map does not allow for duplicates
    if (command_index_map_.count(map_key) == 1) {
        // using .at() due that operator[] is not const
        return command_index_map_.at(map_key);
    } else {
        throw FatalException("CommandMap",
                             "command_index",
                             "bad multi command format, \
                             \nmulti command maps dont allow for duplicates");
    }
}

} // << mzn
