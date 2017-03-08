// ** TxCalibrationDelay Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_TX_CALIBRATION_DELAY_H
#define _MZN_CMD_TX_CALIBRATION_DELAY_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class TxCalibrationDelay : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, TxCalibrationDelay const & cmd);

public:

    explicit TxCalibrationDelay();
    ~TxCalibrationDelay() = default;

    TxCalibrationDelay(TxCalibrationDelay && rhs) = default;
    TxCalibrationDelay & operator=(TxCalibrationDelay && rhs) = default;
    TxCalibrationDelay(TxCalibrationDelay const & rhs) = default;
    TxCalibrationDelay & operator=(TxCalibrationDelay const & rhs) = default;
    std::string const cmd_name = "tx_calibration_delay";

    CmdFieldDuration<uint16_t> calibration_delay;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, TxCalibrationDelay const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_TX_CALIBRATION_DELAY_H
