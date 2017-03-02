#include "c1_dev.h"
namespace mzn {

uint16_t C1Dev::number_of_ic(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) const {

    auto const msg_size = msg.size();
    int constexpr ic_msg_length = 20; // msg length of cx_dev
    return msg_size / ic_msg_length;
}

}
