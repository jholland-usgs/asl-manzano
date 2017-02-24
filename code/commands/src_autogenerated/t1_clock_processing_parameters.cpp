#include "t1_clock_processing_parameters.h"

namespace mzn {
T1ClockProcessingParameters::T1ClockProcessingParameters():
    Command(6, 16),
    timezone_offset(),
    loss_before_downgrading_1_percent_quality(),
    pll_locked_quality(),
    pll_tracking_quality(),
    pll_hold_quality(),
    pll_off_quality(),
    ignore_0(),
    highest_has_been_locked_quality(),
    lowest_has_been_locked_quality(),
    never_been_locked_quality(),
    clock_quality_filter() { }

uint16_t T1ClockProcessingParameters::msg_to_data(std::vector<uint8_t> const & msg,
                                                  uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1ClockProcessingParameters",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = timezone_offset.msg_to_data(msg, mf_begin);
    mf_begin = loss_before_downgrading_1_percent_quality.msg_to_data(msg, mf_begin);
    mf_begin = pll_locked_quality.msg_to_data(msg, mf_begin);
    mf_begin = pll_tracking_quality.msg_to_data(msg, mf_begin);
    mf_begin = pll_hold_quality.msg_to_data(msg, mf_begin);
    mf_begin = pll_off_quality.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 1; // ignore type
    mf_begin = highest_has_been_locked_quality.msg_to_data(msg, mf_begin);
    mf_begin = lowest_has_been_locked_quality.msg_to_data(msg, mf_begin);
    mf_begin = never_been_locked_quality.msg_to_data(msg, mf_begin);
    mf_begin = clock_quality_filter.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1ClockProcessingParameters::data_to_msg(std::vector<uint8_t> & msg,
                                                  uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1ClockProcessingParameters",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = timezone_offset.data_to_msg(msg, mf_begin);
    mf_begin = loss_before_downgrading_1_percent_quality.data_to_msg(msg, mf_begin);
    mf_begin = pll_locked_quality.data_to_msg(msg, mf_begin);
    mf_begin = pll_tracking_quality.data_to_msg(msg, mf_begin);
    mf_begin = pll_hold_quality.data_to_msg(msg, mf_begin);
    mf_begin = pll_off_quality.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 1; // ignore type
    mf_begin = highest_has_been_locked_quality.data_to_msg(msg, mf_begin);
    mf_begin = lowest_has_been_locked_quality.data_to_msg(msg, mf_begin);
    mf_begin = never_been_locked_quality.data_to_msg(msg, mf_begin);
    mf_begin = clock_quality_filter.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1ClockProcessingParameters::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_CLOCK_PROCESSING_PARAMETERS ---  \n";

    cmd_os << "\ntimezone_offset: "; cmd_os << timezone_offset;

    cmd_os << "\nloss_before_downgrading_1_percent_quality: "; cmd_os << loss_before_downgrading_1_percent_quality;

    cmd_os << "\npll_locked_quality: "; cmd_os << pll_locked_quality;

    cmd_os << "\npll_tracking_quality: "; cmd_os << pll_tracking_quality;

    cmd_os << "\npll_hold_quality: "; cmd_os << pll_hold_quality;

    cmd_os << "\npll_off_quality: "; cmd_os << pll_off_quality;

    cmd_os << "\nhighest_has_been_locked_quality: "; cmd_os << highest_has_been_locked_quality;

    cmd_os << "\nlowest_has_been_locked_quality: "; cmd_os << lowest_has_been_locked_quality;

    cmd_os << "\nnever_been_locked_quality: "; cmd_os << never_been_locked_quality;

    cmd_os << "\nclock_quality_filter: "; cmd_os << clock_quality_filter;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace