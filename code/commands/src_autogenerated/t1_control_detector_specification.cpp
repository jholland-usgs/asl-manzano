#include "t1_control_detector_specification.h"

namespace mzn {
T1ControlDetectorSpecification::T1ControlDetectorSpecification():
    CommandVectorNb(131, 4),
    number_of_bytes(),
    detector_number(),
    detector_option(),
    detector_name() { }

uint16_t T1ControlDetectorSpecification::msg_to_data(std::vector<uint8_t> const & msg,
                                                     uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1ControlDetectorSpecification",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.msg_to_data(msg, mf_begin);
    mf_begin = detector_number.msg_to_data(msg, mf_begin);
    mf_begin = detector_option.msg_to_data(msg, mf_begin);
    mf_begin = detector_name.msg_to_data(msg, mf_begin);
    mf_begin = CommandVectorNb::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1ControlDetectorSpecification::data_to_msg(std::vector<uint8_t> & msg,
                                                     uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1ControlDetectorSpecification",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.data_to_msg(msg, mf_begin);
    mf_begin = detector_number.data_to_msg(msg, mf_begin);
    mf_begin = detector_option.data_to_msg(msg, mf_begin);
    mf_begin = detector_name.data_to_msg(msg, mf_begin);
    mf_begin = CommandContainer::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1ControlDetectorSpecification::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_CONTROL_DETECTOR_SPECIFICATION ---  \n";

    cmd_os << "\nnumber_of_bytes: "; cmd_os << number_of_bytes;

    cmd_os << "\ndetector_number: "; cmd_os << detector_number;

    cmd_os << "\ndetector_option: "; cmd_os << detector_option;

    cmd_os << "\ndetector_name: "; cmd_os << detector_name;
    cmd_os << std::endl;

    return CommandContainer::os_print(cmd_os);
}


void T1ControlDetectorSpecification::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<TxDetectorEquation>() } );

    if (inner_commands.back() == nullptr) {
        throw WarningException("TxDetectorEquation",
                               "create_new_ic",
                               "nullptr inner command");
     }

}
} // end namespace