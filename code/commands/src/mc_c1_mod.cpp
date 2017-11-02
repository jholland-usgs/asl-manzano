#include "c1_mod.h"
namespace mzn {

// -------------------------------------------------------------------------- //
uint16_t C1Mod::ni(std::vector<uint8_t> const & msg,
                   uint16_t mf_begin) const {

    auto const msg_size = msg.size();
    int constexpr ic_msg_length = 20; // msg length of cx_mod
    return msg_size / ic_msg_length;
}

}
