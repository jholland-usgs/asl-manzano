// Manzano software
#ifndef _MZN_TOKENS_MANAGER_H
#define _MZN_TOKENS_MANAGER_H

#include "seismic_network.h"
#include "message_dispatch.h"
#include "commands.h"

namespace mzn {

//! utility functions to lookup/modify tokens from a T2Tokens object
/*!
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class TokensManager {

public:
    explicit
    TokensManager(T2Tokens & tokens);
    ~TokensManager() = default;

    std::string network_name();

private:
    T2Tokens & tokens;
};

} // end namespace
#endif // _MZN_TOKENS_MANAGER_H_
