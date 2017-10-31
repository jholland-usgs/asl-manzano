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
void TokenManager::set_hf_on() {
    // find a better way, like how the keys works in command map
    bool found = false;
    for (auto & token : tokens.inner_commands) {
        auto * channel = dynamic_cast<T1LogicalChannelQueue *>( token.get() );
        if (channel != nullptr) {
            std::cout << *channel;
            std::cout << channel->option;
            bool hf_on = not channel->option.dont_include_this_lcq_when_calculating_freq_bitmap();
            std::cout << "\nhf_on:" << std::boolalpha << hf_on << std::noboolalpha;
            found = true;
        }
    }

    if (not found) {
        throw WarningException("TokenManager", "set_hf_on", "token not found");
    }
}

// -------------------------------------------------------------------------- //
void TokenManager::set_hf_off() {
    std::cout << "TODO!";
}

} // end namespace
