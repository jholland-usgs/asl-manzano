#include "tokens_manager.h"

namespace mzn {

// -------------------------------------------------------------------------- //
TokensManager::TokensManager(T2Tokens & tokens) : tokens{tokens} {}

// -------------------------------------------------------------------------- //
std::string TokensManager::network_name() {
    std::string hello = "hello";
    return hello;
}

} // end namespace
