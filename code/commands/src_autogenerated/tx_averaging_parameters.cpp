#include "tx_averaging_parameters.h"

namespace mzn {
TxAveragingParameters::TxAveragingParameters():
    Command(0, 5),
    number_of_samples_between_reports(),
    optional_irr_filter_to_use() { }

uint16_t TxAveragingParameters::msg_to_data(std::vector<uint8_t> const & msg,
                                            uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxAveragingParameters",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_samples_between_reports.msg_to_data(msg, mf_begin);
    mf_begin = optional_irr_filter_to_use.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxAveragingParameters::data_to_msg(std::vector<uint8_t> & msg,
                                            uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxAveragingParameters",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_samples_between_reports.data_to_msg(msg, mf_begin);
    mf_begin = optional_irr_filter_to_use.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxAveragingParameters::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_AVERAGING_PARAMETERS ---  \n";

    cmd_os << "\nnumber_of_samples_between_reports: "; cmd_os << number_of_samples_between_reports;

    cmd_os << "\noptional_irr_filter_to_use: "; cmd_os << optional_irr_filter_to_use;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace