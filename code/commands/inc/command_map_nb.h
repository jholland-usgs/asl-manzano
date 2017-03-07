// ** CommandMapNb Class ** //
// Manzano software

#ifndef _MZN_COMMAND_MAP_NB_H
#define _MZN_COMMAND_MAP_NB_H

#include "command_map.h"
#include <map>

namespace mzn {
//! CommandMap when the inner commands are setup as a map
/*!

    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class CommandMapNb : public CommandMap {

public:

    CommandMapNb(uint16_t const cmd_number,
                 uint16_t const cmd_data_size);

    ~CommandMapNb() = default;
    CommandMapNb(CommandMapNb && rhs) noexcept;
    CommandMapNb & operator=(CommandMapNb && rhs) noexcept;

    //! Pure virtual,
    //! NOT auto generated in derived classes
    virtual
    uint16_t nb(std::vector<uint8_t> const & msg,
                uint16_t mf_begin) const = 0;

protected:

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    //! Calls data_to_msg on all inner commands
    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;
};

} // << mzn
#endif // _MZN_COMMAND_MAP_NB_
