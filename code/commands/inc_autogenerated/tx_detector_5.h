// ** TxDetector5 Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_TX_DETECTOR_5_H
#define _MZN_CMD_TX_DETECTOR_5_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class TxDetector5 : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, TxDetector5 const & cmd);

public:

    explicit TxDetector5();
    ~TxDetector5() = default;

    TxDetector5(TxDetector5 && rhs) = default;
    TxDetector5 & operator=(TxDetector5 && rhs) = default;
    TxDetector5(TxDetector5 const & rhs) = default;
    TxDetector5 & operator=(TxDetector5 const & rhs) = default;
    std::string const cmd_name = "tx_detector_5";

    CmdField<uint8_t> detector_number_to_use_as_base;
    CmdField<uint8_t> detector_number_of_this_invocation;
    BmDetectorOptions detector_options;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, TxDetector5 const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_TX_DETECTOR_5_H
