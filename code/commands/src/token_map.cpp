// ** TokenMap Class ** //
// Manzano software
#include "token_map.h"

namespace mzn {

// -------------------------------------------------------------------------- //
TokenMap::TokenMap() = default;

// -------------------------------------------------------------------------- //
template <>
inline
uint16_t TokenMap::token_size(Command const & tk) const {

    return tk.cmd_data_size();
}
} // << mzn
