#include "token_manager.h"

namespace mzn {

// -------------------------------------------------------------------------- //
TokenManager::TokenManager(std::unique_ptr<T2Tokens> & tokens_ptr) :
        tokens{*tokens_ptr} {}

// -------------------------------------------------------------------------- //
std::string TokenManager::network_name() const {
    std::string hello = "hello";
    return hello;
}

// -------------------------------------------------------------------------- //
void TokenManager::set_channel(std::string const & location_code,
                               std::string const & seed_name,
                               bool const on) {
    bool found = false;

    CmdFieldArrayChar<2> cf_location_code(location_code);
    CmdFieldArrayChar<3> cf_seed_name(seed_name);
    // find a better way, like how the keys works in command map
    for (auto & token : tokens.inner_commands) {
        auto * channel = dynamic_cast<T1LogicalChannelQueue *>( token.get() );
        if (channel != nullptr) {
            if (channel->location_code == cf_location_code and
                channel->seed_name == cf_seed_name) {
                // these two variables should identify a channel
                found = true;
                std::cout << *channel;
                std::cout << channel->option;
                bool hf_on = not channel->option.dont_include_this_lcq_when_calculating_freq_bitmap();
                std::cout << "\nhf is:" << std::boolalpha << hf_on << std::noboolalpha;
                std::cout << "\nhf want:" << std::boolalpha << on << std::noboolalpha;
                if (hf_on != on) {
                    channel->option.dont_include_this_lcq_when_calculating_freq_bitmap(not on);
                    std::cout << "\nchannel changed";
                }
            }
        }
    }

    if (not found) {
        throw WarningException("TokenManager", "set_hf_on", "token not found");
    }
}

// -------------------------------------------------------------------------- //
void TokenManager::set_hf_on(Sensor::Input const & input) {
    if (input == Sensor::Input::a) {
        set_channel("CB", "BC0", true);
        set_channel("EHZ", "00", true);
    } else {
        set_channel("CB", "BC1", true);
        set_channel("EHZ", "10", true);
    }
}

// -------------------------------------------------------------------------- //
void TokenManager::set_hf_off(Sensor::Input const & input) {
    if (input == Sensor::Input::a) {
        set_channel("CB", "BC0", false);
        set_channel("EHZ", "00", false);
    } else {
        set_channel("CB", "BC1", false);
        set_channel("EHZ", "10", false);
    }
}

} // end namespace
