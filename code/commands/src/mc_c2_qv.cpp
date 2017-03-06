#include "c2_qv.h"

namespace mzn {

// -------------------------------------------------------------------------- //
uint16_t C2Qv::ni(std::vector<uint8_t> const & msg,
                  uint16_t mf_begin) const {
    // header size = 0
    return this->number_of_entries();
}
}
