// ** CommandMapNi Class ** //
// Manzano software

#ifndef _MZN_COMMAND_MAP_NI_H
#define _MZN_COMMAND_MAP_NI_H

#include "command_map.h"
#include <map>

namespace mzn {
//! CommandMap when the inner commands are setup as a map
/*!

    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class CommandMapNi : public CommandMap {

public:

    CommandMapNi(uint16_t const cmd_number,
                 uint16_t const cmd_data_size);

    ~CommandMapNi() = default;
    CommandMapNi(CommandMapNi && rhs) noexcept;
    CommandMapNi & operator=(CommandMapNi && rhs) noexcept;

    //! pure, not auto generated, typically checks cmd header
    //! @return true when a particular map_key is expected on the msg
    //! @see mc_c1_stat.cpp
    virtual
    bool command_active(uint8_t const map_key) const = 0;


protected:

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;
};

} // << mzn
#endif // _MZN_COMMAND_MAP_NI_
