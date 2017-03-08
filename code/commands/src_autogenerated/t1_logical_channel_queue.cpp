#include "t1_logical_channel_queue.h"

namespace mzn {
T1LogicalChannelQueue::T1LogicalChannelQueue():
    CommandMapNi(128, 15),
    number_of_bytes(),
    location_code(),
    seed_name(),
    lcq_reference_number(),
    source(),
    option(),
    rate() { }

uint16_t T1LogicalChannelQueue::msg_to_data(std::vector<uint8_t> const & msg,
                                            uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1LogicalChannelQueue",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.msg_to_data(msg, mf_begin);
    mf_begin = location_code.msg_to_data(msg, mf_begin);
    mf_begin = seed_name.msg_to_data(msg, mf_begin);
    mf_begin = lcq_reference_number.msg_to_data(msg, mf_begin);
    mf_begin = source.msg_to_data(msg, mf_begin);
    mf_begin = option.msg_to_data(msg, mf_begin);
    mf_begin = rate.msg_to_data(msg, mf_begin);
    mf_begin = CommandMapNi::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T1LogicalChannelQueue::data_to_msg(std::vector<uint8_t> & msg,
                                            uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "T1LogicalChannelQueue",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_bytes.data_to_msg(msg, mf_begin);
    mf_begin = location_code.data_to_msg(msg, mf_begin);
    mf_begin = seed_name.data_to_msg(msg, mf_begin);
    mf_begin = lcq_reference_number.data_to_msg(msg, mf_begin);
    mf_begin = source.data_to_msg(msg, mf_begin);
    mf_begin = option.data_to_msg(msg, mf_begin);
    mf_begin = rate.data_to_msg(msg, mf_begin);
    mf_begin = CommandContainer::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T1LogicalChannelQueue::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T1_LOGICAL_CHANNEL_QUEUE ---  \n";

    cmd_os << "\nnumber_of_bytes: "; cmd_os << number_of_bytes;

    cmd_os << "\nlocation_code: "; cmd_os << location_code;

    cmd_os << "\nseed_name: "; cmd_os << seed_name;

    cmd_os << "\nlcq_reference_number: "; cmd_os << lcq_reference_number;

    cmd_os << "\nsource: "; cmd_os << source;

    cmd_os << "\noption: "; cmd_os << option;

    cmd_os << "\nrate: "; cmd_os << rate;
    cmd_os << std::endl;

    return CommandContainer::os_print(cmd_os);
}


void T1LogicalChannelQueue::create_new_ic(uint8_t const cmd_key) {

    using CKE = T1LogicalChannelQueue::Keys;
    auto const cmd_key_enum = static_cast<CKE>(cmd_key);

    switch(cmd_key_enum) {
        case CKE::tx_pre_event_buffers : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxPreEventBuffers>() } );
            break;
        }
        case CKE::tx_gap_threshold : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxGapThreshold>() } );
            break;
        }
        case CKE::tx_calibration_delay : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxCalibrationDelay>() } );
            break;
        }
        case CKE::tx_maximum_frame_count : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxMaximumFrameCount>() } );
            break;
        }
        case CKE::tx_fir_multiplier : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxFirMultiplier>() } );
            break;
        }
        case CKE::tx_averaging_parameters : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxAveragingParameters>() } );
            break;
        }
        case CKE::tx_control_detector : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxControlDetector>() } );
            break;
        }
        case CKE::tx_decimation_entry : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxDecimationEntry>() } );
            break;
        }
        case CKE::tx_detector_1 : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxDetector1>() } );
            break;
        }
        case CKE::tx_detector_2 : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxDetector2>() } );
            break;
        }
        case CKE::tx_detector_3 : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxDetector3>() } );
            break;
        }
        case CKE::tx_detector_4 : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxDetector4>() } );
            break;
        }
        case CKE::tx_detector_5 : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxDetector5>() } );
            break;
        }
        case CKE::tx_detector_6 : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxDetector6>() } );
            break;
        }
        case CKE::tx_detector_7 : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxDetector7>() } );
            break;
        }
        case CKE::tx_detector_8 : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<TxDetector8>() } );
            break;
        }
    }

    if (inner_commands.back() == nullptr) {
        throw WarningException("T1LogicalChannelQueue",
                               "create_new_ic",
                               "nullptr inner command: " + std::to_string(cmd_key) );
     }

}
} // end namespace