#include "t1_configuration_identification.h"

namespace mzn {
T1ConfigurationIdentification::T1ConfigurationIdentification():
    Command(8, 8),
    configuration_stream_seed_location(),
    configuration_stream_seed_name(),
    flags(),
    interval() { }

uint16_t T1ConfigurationIdentification::msg_to_data(std::vector<uint8_t> const & msg,
                                                    uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1ConfigurationIdentification",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = configuration_stream_seed_location.msg_to_data(msg, mf_begin);
    mf_begin = configuration_stream_seed_name.msg_to_data(msg, mf_begin);
    mf_begin = flags.msg_to_data(msg, mf_begin);
    mf_begin = interval.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1ConfigurationIdentification::data_to_msg(std::vector<uint8_t> & msg,
                                                    uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1ConfigurationIdentification",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = configuration_stream_seed_location.data_to_msg(msg, mf_begin);
    mf_begin = configuration_stream_seed_name.data_to_msg(msg, mf_begin);
    mf_begin = flags.data_to_msg(msg, mf_begin);
    mf_begin = interval.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1ConfigurationIdentification::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_CONFIGURATION_IDENTIFICATION ---  \n";

    cmd_os << "\nconfiguration_stream_seed_location: "; cmd_os << configuration_stream_seed_location;

    cmd_os << "\nconfiguration_stream_seed_name: "; cmd_os << configuration_stream_seed_name;

    cmd_os << "\nflags: "; cmd_os << flags;

    cmd_os << "\ninterval: "; cmd_os << interval;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace