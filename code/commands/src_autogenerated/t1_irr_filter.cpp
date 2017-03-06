#include "t1_irr_filter.h"

namespace mzn {
T1IrrFilter::T1IrrFilter():
    CommandVectorNi(129, 12),
    number_of_bytes(),
    filter_number(),
    filter_name(),
    number_of_sections(),
    gain(),
    reference_frequency() { }

uint16_t T1IrrFilter::msg_to_data(std::vector<uint8_t> const & msg,
                                  uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1IrrFilter",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.msg_to_data(msg, mf_begin);
    mf_begin = filter_number.msg_to_data(msg, mf_begin);
    mf_begin = filter_name.msg_to_data(msg, mf_begin);
    mf_begin = number_of_sections.msg_to_data(msg, mf_begin);
    mf_begin = gain.msg_to_data(msg, mf_begin);
    mf_begin = reference_frequency.msg_to_data(msg, mf_begin);
    mf_begin = CommandVectorNi::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1IrrFilter::data_to_msg(std::vector<uint8_t> & msg,
                                  uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1IrrFilter",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.data_to_msg(msg, mf_begin);
    mf_begin = filter_number.data_to_msg(msg, mf_begin);
    mf_begin = filter_name.data_to_msg(msg, mf_begin);
    mf_begin = number_of_sections.data_to_msg(msg, mf_begin);
    mf_begin = gain.data_to_msg(msg, mf_begin);
    mf_begin = reference_frequency.data_to_msg(msg, mf_begin);
    mf_begin = CommandContainer::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1IrrFilter::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_IRR_FILTER ---  \n";

    cmd_os << "\nnumber_of_bytes: "; cmd_os << number_of_bytes;

    cmd_os << "\nfilter_number: "; cmd_os << filter_number;

    cmd_os << "\nfilter_name: "; cmd_os << filter_name;

    cmd_os << "\nnumber_of_sections: "; cmd_os << number_of_sections;

    cmd_os << "\ngain: "; cmd_os << gain;

    cmd_os << "\nreference_frequency: "; cmd_os << reference_frequency;
    cmd_os << std::endl;

    return CommandContainer::os_print(cmd_os);
}


void T1IrrFilter::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<TxIrrFilter>() } );

    if (inner_commands.back() == nullptr) {
        throw WarningException("TxIrrFilter",
                               "create_new_ic",
                               "nullptr inner command");
     }

}
} // end namespace