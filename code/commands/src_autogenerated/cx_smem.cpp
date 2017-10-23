#include "cx_smem.h"

namespace mzn {
CxSmem::CxSmem():
    Command(0, 4),
    segment_number(),
    total_number_of_segments(),
    memory_contents() { }

uint16_t CxSmem::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxSmem",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = segment_number.msg_to_data(msg, mf_begin);
    mf_begin = total_number_of_segments.msg_to_data(msg, mf_begin);
    mf_begin = memory_contents.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxSmem::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxSmem",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = segment_number.data_to_msg(msg, mf_begin);
    mf_begin = total_number_of_segments.data_to_msg(msg, mf_begin);
    mf_begin = memory_contents.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxSmem::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_SMEM ---  \n";

    cmd_os << "\nsegment_number: "; cmd_os << segment_number;

    cmd_os << "\ntotal_number_of_segments: "; cmd_os << total_number_of_segments;

    cmd_os << "\nmemory_contents: "; cmd_os << memory_contents;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace