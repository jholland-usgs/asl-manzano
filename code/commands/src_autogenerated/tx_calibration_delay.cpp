#include "tx_calibration_delay.h"

namespace mzn {
TxCalibrationDelay::TxCalibrationDelay():
    Command(0, 2),
    calibration_delay() { }

uint16_t TxCalibrationDelay::msg_to_data(std::vector<uint8_t> const & msg,
                                         uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxCalibrationDelay",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = calibration_delay.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t TxCalibrationDelay::data_to_msg(std::vector<uint8_t> & msg,
                                         uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "TxCalibrationDelay",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = calibration_delay.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & TxCalibrationDelay::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- TX_CALIBRATION_DELAY ---  \n";

    cmd_os << "\ncalibration_delay: "; cmd_os << calibration_delay;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace