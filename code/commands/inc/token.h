// ** TokenMap Class ** //
// Manzano software

#ifndef _MZN_TOKEN_MAP_H
#define _MZN_TOKEN_MAP_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <unordered_map>

#include "cmd_field.h"
#include "cmd_field_array_byte.h"
#include "cmd_field_array_char.h"
#include "cmd_field_array_ubyte.h"
#include "cmd_field_bitmap.h"
#include "cmd_field_bitmap_types.h"
#include "cmd_field_cal_amplitude.h"
#include "cmd_field_hex.h"
#include "cmd_field_pstring.h"
#include "cmd_field_ignore.h"
#include "cmd_field_ip.h"
#include "cmd_field_time.h"
#include "cmd_field_duration.h"

#include "mzn_except.h"

namespace mzn {

//! Base for all tokens to/from digitizer
/*!
    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class Token {

public:

    // --------------------------------------------------------------------- //
    Token();
    ~Token() = default;

    //! calls data_to_msg on token fields in order
    //! @throws logic msg too short
    // --------------------------------------------------------------------- //
    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin = 0) const noexcept(false);

    //! calls msg_to_data on token fields in order
    //! @throws logic msg too short
    // --------------------------------------------------------------------- //
    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin = 0) noexcept(false);
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & token_os,
                          Token const & token) {

    return token_os;
}

} // << mzn
#endif // _MZN_TOKEN_MAP_H_
