#include "t1_ignore.h"

namespace mzn {
T1Ignore::T1Ignore():
    Command(0, 0) { }

uint16_t T1Ignore::msg_to_data(std::vector<uint8_t> const & msg,
                               uint16_t mf_begin) {


    return mf_begin;
}

uint16_t T1Ignore::data_to_msg(std::vector<uint8_t> & msg,
                               uint16_t mf_begin) const {


    return mf_begin;
}

std::ostream & T1Ignore::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_IGNORE ---  \n";
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace