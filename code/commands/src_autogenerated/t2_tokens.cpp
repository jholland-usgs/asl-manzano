#include "t2_tokens.h"

namespace mzn {
T2Tokens::T2Tokens():
    CommandMapNb(0, 0) { }

uint16_t T2Tokens::msg_to_data(std::vector<uint8_t> const & msg,
                               uint16_t mf_begin) {

    mf_begin = CommandMapNb::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t T2Tokens::data_to_msg(std::vector<uint8_t> & msg,
                               uint16_t mf_begin) const {

    mf_begin = CommandMapNb::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & T2Tokens::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- T2_TOKENS ---  \n";
    cmd_os << std::endl;

    return CommandContainer::os_print(cmd_os);
}


void T2Tokens::create_new_ic(uint8_t const cmd_key) {

    using CKE = T2Tokens::Keys;
    auto const cmd_key_enum = static_cast<CKE>(cmd_key);

    switch(cmd_key_enum) {
        case CKE::t1_ignore : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1Ignore>() } );
            break;
        }
        case CKE::t1_version_number : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1VersionNumber>() } );
            break;
        }
        case CKE::t1_network_station : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1NetworkStation>() } );
            break;
        }
        case CKE::t1_dp_netserver : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1DpNetserver>() } );
            break;
        }
        case CKE::t1_data_subscription_server : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1DataSubscriptionServer>() } );
            break;
        }
        case CKE::t1_dp_webserver : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1DpWebserver>() } );
            break;
        }
        case CKE::t1_clock_processing : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1ClockProcessing>() } );
            break;
        }
        case CKE::t1_log_and_timing : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1LogAndTiming>() } );
            break;
        }
        case CKE::t1_configuration_identification : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1ConfigurationIdentification>() } );
            break;
        }
        case CKE::t1_data_server : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1DataServer>() } );
            break;
        }
        case CKE::t1_comm_event : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1CommEvent>() } );
            break;
        }
        case CKE::t1_logical_channel_queue : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1LogicalChannelQueue>() } );
            break;
        }
        case CKE::t1_irr_filter : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1IrrFilter>() } );
            break;
        }
        case CKE::t1_fir_filter : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1FirFilter>() } );
            break;
        }
        case CKE::t1_control_detector_specification : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1ControlDetectorSpecification>() } );
            break;
        }
        case CKE::t1_murdock_hutt_detector : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1MurdockHuttDetector>() } );
            break;
        }
        case CKE::t1_threshold_detector : {

            inner_commands.push_back(
                std::unique_ptr<Command>{
                     std::make_unique<T1ThresholdDetector>() } );
            break;
        }
    }

    if (inner_commands.back() == nullptr) {
        throw WarningException("T2Tokens",
                               "create_new_ic",
                               "nullptr inner command: " + std::to_string(cmd_key) );
     }

}
} // end namespace