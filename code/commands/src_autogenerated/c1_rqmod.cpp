#include "c1_rqmod.h"

namespace mzn {
C1Rqmod::C1Rqmod():
    Command(0x43, 0) { }

uint16_t C1Rqmod::msg_to_data(std::vector<uint8_t> const & msg,
                              uint16_t mf_begin) {


    return mf_begin;
}

uint16_t C1Rqmod::data_to_msg(std::vector<uint8_t> & msg,
                              uint16_t mf_begin) const {


    return mf_begin;
}

std::ostream & C1Rqmod::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_RQMOD ---  \n";
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace