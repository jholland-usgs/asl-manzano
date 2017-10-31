#include "token_manager.h"

namespace mzn {

// -------------------------------------------------------------------------- //
TokenManager::TokenManager(std::unique_ptr<T2Tokens> & tokens_ptr) :
        tokens{*tokens_ptr} {}

// -------------------------------------------------------------------------- //
std::string TokenManager::network_name() const {
    // should actually just be the first in inner_commands
    std::string hello = "TODO!";
    return hello;
}

// -------------------------------------------------------------------------- //
void TokenManager::stream_channels(std::ostream & os) {
    for (auto & token : tokens.inner_commands) {
        auto * channel = dynamic_cast<T1LogicalChannelQueue *>( token.get() );
        if (channel != nullptr) {
            os << "\n" << channel->location_code
               << channel->seed_name;
        }
    }
}

// -------------------------------------------------------------------------- //
bool TokenManager::set_channel(std::string const & location_code,
                               std::string const & seed_name,
                               bool const on) {
    bool found = false;
    bool changed = false;

    CmdFieldArrayChar<2> cf_location_code(location_code);
    CmdFieldArrayChar<3> cf_seed_name(seed_name);
    // find a better way, like how the keys works in command map
    for (auto & token : tokens.inner_commands) {
        auto * channel = dynamic_cast<T1LogicalChannelQueue *>( token.get() );
        if (channel != nullptr) {
            // these two variables should identify a channel
            if (channel->location_code == cf_location_code and
                channel->seed_name == cf_seed_name) {
                std::cout << *channel;
                bool hf_on = not channel->option.dont_include_this_lcq_when_calculating_freq_bitmap();
                std::cout << "\nhf is:" << std::boolalpha << hf_on << std::noboolalpha;
                std::cout << "\nhf want:" << std::boolalpha << on << std::noboolalpha;
                if (hf_on != on) {
                    channel->option.dont_include_this_lcq_when_calculating_freq_bitmap(not on);
                    changed = true;
                    std::cout << "\nchannel changed";
                }
                found = true;
                break;
            }
        }
    }

    if (not found) {
        std::stringstream ss;
        ss << "token for channel: " << location_code << "/" << seed_name
           << " not found";
        throw WarningException( "TokenManager", "set_hf_on", ss.str() );
    }

    return changed;
}

// -------------------------------------------------------------------------- //
bool TokenManager::set_hf_on(Sensor::Input const & input) {
    bool changed = false;
    if (input == Sensor::Input::a) {
        changed = changed or set_channel("CB", "BC0", true);
        changed = changed or set_channel("00", "EH1", true);
        changed = changed or set_channel("00", "EH2", true);
        changed = changed or set_channel("00", "EHZ", true);
    } else {
        changed = changed or set_channel("CB", "BC1", true);
        changed = changed or set_channel("10", "EH1", true);
        changed = changed or set_channel("10", "EH2", true);
        changed = changed or set_channel("10", "EHZ", true);
    }
    return changed;
}

// -------------------------------------------------------------------------- //
bool TokenManager::set_hf_off(Sensor::Input const & input) {
    bool changed = false;
    if (input == Sensor::Input::a) {
        changed = changed or set_channel("CB", "BC0", false);
        changed = changed or set_channel("00", "EH1", false);
        changed = changed or set_channel("00", "EH2", false);
        changed = changed or set_channel("00", "EHZ", false);
    } else {
        changed = changed or set_channel("CB", "BC1", false);
        changed = changed or set_channel("10", "EH1", false);
        changed = changed or set_channel("10", "EH2", false);
        changed = changed or set_channel("10", "EHZ", false);
    }
    return changed;
}

} // end namespace
