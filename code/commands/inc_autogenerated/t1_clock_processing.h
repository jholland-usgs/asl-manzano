// ** T1ClockProcessing Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_T1_CLOCK_PROCESSING_H
#define _MZN_CMD_T1_CLOCK_PROCESSING_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class T1ClockProcessing : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, T1ClockProcessing const & cmd);

public:

    explicit T1ClockProcessing();
    ~T1ClockProcessing() = default;

    T1ClockProcessing(T1ClockProcessing && rhs) = default;
    T1ClockProcessing & operator=(T1ClockProcessing && rhs) = default;
    T1ClockProcessing(T1ClockProcessing const & rhs) = default;
    T1ClockProcessing & operator=(T1ClockProcessing const & rhs) = default;
    std::string const cmd_name = "t1_clock_processing";

    CmdFieldDuration<int32_t> timezone_offset;
    CmdFieldDuration<uint16_t, std::ratio<60>  > loss_before_downgrading_1_percent_quality;
    CmdField<int8_t> pll_locked_quality;
    CmdField<int8_t> pll_tracking_quality;
    CmdField<int8_t> pll_hold_quality;
    CmdField<int8_t> pll_off_quality;
    CmdFieldIgnore<uint8_t> ignore_0;
    CmdField<int8_t> highest_has_been_locked_quality;
    CmdField<int8_t> lowest_has_been_locked_quality;
    CmdField<int8_t> never_been_locked_quality;
    CmdFieldDuration<uint16_t> clock_quality_filter;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, T1ClockProcessing const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_T1_CLOCK_PROCESSING_H
