#include "c1_mod.h"

namespace mzn {
C1Mod::C1Mod():
    CommandVectorNi(0xB9, 0) { }

uint16_t C1Mod::msg_to_data(std::vector<uint8_t> const & msg,
                            uint16_t mf_begin) {

    mf_begin = CommandVectorNi::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Mod::data_to_msg(std::vector<uint8_t> & msg,
                            uint16_t mf_begin) const {

    mf_begin = CommandContainer::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Mod::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_MOD ---  \n";
    cmd_os << std::endl;

    return CommandContainer::os_print(cmd_os);
}


void C1Mod::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<CxMod>() } );

    if (inner_commands.back() == nullptr) {
        throw WarningException("CxMod",
                               "create_new_ic",
                               "nullptr inner command");
     }

}
} // end namespace