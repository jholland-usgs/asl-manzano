#include "tokens_manager.h"

namespace mzn {

// -------------------------------------------------------------------------- //
TokensManager::TokensManager(T2Tokens & tokens) : tokens{tokens} {}

// -------------------------------------------------------------------------- //
std::string TokensManager::network_name() const {
    std::string hello = "hello";
    return hello;
}

// -------------------------------------------------------------------------- //
void TokensManager::set_hf_on() {

    // find a better way, like how the keys works in command map
    bool found = false;
    for (auto & token : tokens.inner_commands) {
        auto * t = dynamic_cast<T1LogicalChannelQueue *>( token.get() );
        if (t != nullptr) {
            std::cout << *t;
            std::cout << t->option;
            bool hf_on = not t->option.dont_include_this_lcq_when_calculating_freq_bitmap();
            std::cout << "\nhf_on:" << std::boolalpha << hf_on << std::noboolalpha;
            found = true;
        }
    }

    if (not found) {
        throw WarningException("TokensManager", "set_hf_on", "token not found");
    }

}

// -------------------------------------------------------------------------- //
void TokensManager::set_hf_off() {
    std::cout << "TODO!";
}

} // end namespace
