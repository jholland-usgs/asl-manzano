// ** CommandMap Class ** //
// Manzano software

#ifndef _MZN_COMMAND_MAP_H
#define _MZN_COMMAND_MAP_H

#include "command_container.h"
#include <map>

namespace mzn {
//! CommandContainer when the inner commands are setup as a map
/*!
    This class works for a fixed number of inner_command(s) possible
    So that each inner_command is expected and is just checked at runtime
    with command_active.

    The storage of the commands is still store on CommandContainer's
    inner_commands vector, however since potentially the commands stored will be
    of different types (or more accuratly unique_ptr of different types to
    use polymorphism), a map is provided here to map the original type
    of command stored to the vector element index (command_index);

    The type of the command is stored as a number here (map_key), and is casted to
    an enum class inside the CommandMap derived class (auto generated)

    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class CommandMap : public CommandContainer {

public:

    CommandMap(uint16_t const cmd_number, uint16_t const cmd_data_size);
    ~CommandMap() = default;
    CommandMap(CommandMap && rhs) noexcept;
    CommandMap & operator=(CommandMap && rhs) noexcept;
    //CommandMap & operator=(CommandMap const && rhs) const noexcept {};
    //CommandMap(CommandMap const &) = default;
    virtual
    std::vector<uint8_t> const keys() const = 0;

protected:

    //! @return the cmd_index in the map
    //! @throw if map_key is not found on the map
    int command_index(uint8_t const map_key) const;

    //! inserts <map_key, cmd_index> on command_index_map_
    void set_command_index(uint8_t const map_key,
                           int const cmd_index);

    //! map <map_key, cmd_index>
    std::map <uint8_t, int> command_index_map_;
};

} // << mzn
#endif // _MZN_COMMAND_MAP_
