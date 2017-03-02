// ** MultiCommandMap Class ** //
// Manzano software

#ifndef _MZN_MULTI_COMMAND_MAP_H
#define _MZN_MULTI_COMMAND_MAP_H

#include "multi_command.h"
#include <map>

namespace mzn {
//! MultiCommand when the inner commands are setup as a map
/*!
    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class MultiCommandMap : public MultiCommand {

public:

    MultiCommandMap(uint16_t const cmd_number,
                    uint16_t const cmd_data_size);

    ~MultiCommandMap() = default;
    MultiCommandMap(MultiCommandMap && rhs) noexcept;
    MultiCommandMap & operator=(MultiCommandMap && rhs) noexcept;
    //MultiCommandMap & operator=(MultiCommandMap const && rhs) const noexcept {};
    //MultiCommandMap(MultiCommandMap const &) = default;

protected:

    //! pure, not auto generated, typically checks cmd header
    //! @return true when a particular map_key is expected on the msg
    //! @see mc_c1_stat.cpp
    virtual
    bool command_active(uint8_t const map_key) const = 0;

    //! @return the cmd_index in the map
    int command_index(uint8_t const map_key) const;

    //! @called inside create_new_ic
    void set_command_index(uint8_t const map_key,
                           int const cmd_index);

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    //! map <map_key, cmd_index>
    std::map <uint8_t, int> command_index_map_;
};

} // << mzn
#endif // _MZN_MULTI_COMMAND_MAP_
