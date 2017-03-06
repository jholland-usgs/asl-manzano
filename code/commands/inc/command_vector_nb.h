// ** CommandVectorNb Class ** //
// Manzano software

#ifndef _MZN_COMMAND_VECTOR_NB_H
#define _MZN_COMMAND_VECTOR_NB_H

#include "command_container.h"
#include <map>

namespace mzn {
//! CommandContainer when the inner commands are setup as a map
/*!
    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class CommandVectorNb : public CommandContainer {

public:

    CommandVectorNb(uint16_t const cmd_number,
                    uint16_t const cmd_data_size);

    ~CommandVectorNb() = default;
    CommandVectorNb(CommandVectorNb && rhs) noexcept;
    CommandVectorNb & operator=(CommandVectorNb && rhs) noexcept;

    //! Pure virtual,
    //! NOT auto generated in derived classes
    virtual
    uint16_t nb(std::vector<uint8_t> const & msg,
                uint16_t mf_begin) const = 0;

protected:

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;
};

} // << mzn
#endif // _MZN_COMMAND_VECTOR_NB_
