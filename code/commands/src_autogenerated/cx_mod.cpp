#include "cx_mod.h"

namespace mzn {
CxMod::CxMod():
    Command(0, 20),
    module_name(),
    revision(),
    overlay_number(),
    module_crc() { }

uint16_t CxMod::msg_to_data(std::vector<uint8_t> const & msg,
                            uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxMod",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = module_name.msg_to_data(msg, mf_begin);
    mf_begin = revision.msg_to_data(msg, mf_begin);
    mf_begin = overlay_number.msg_to_data(msg, mf_begin);
    mf_begin = module_crc.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxMod::data_to_msg(std::vector<uint8_t> & msg,
                            uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxMod",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = module_name.data_to_msg(msg, mf_begin);
    mf_begin = revision.data_to_msg(msg, mf_begin);
    mf_begin = overlay_number.data_to_msg(msg, mf_begin);
    mf_begin = module_crc.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxMod::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_MOD ---  \n";

    cmd_os << "\nmodule_name: "; cmd_os << module_name;

    cmd_os << "\nrevision: "; cmd_os << revision;

    cmd_os << "\noverlay_number: "; cmd_os << overlay_number;

    cmd_os << "\nmodule_crc: "; cmd_os << module_crc;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace