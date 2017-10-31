// Manzano software
#ifndef _MZN_TOKENS_MANAGER_H
#define _MZN_TOKENS_MANAGER_H

#include "seismic_network.h"
#include "output_store.h"
#include "commands.h"

namespace mzn {
//! utility functions to lookup/modify tokens from a T2Tokens object
/*!
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class TokenManager {

public:
    explicit
    TokenManager(std::unique_ptr<T2Tokens> & tokens);
    ~TokenManager() = default;

    std::string network_name() const;
    void set_hf_on(Sensor::Input const & input);
    void set_hf_off(Sensor::Input const & input);
    void set_channel(std::string const & location_code,
                     std::string const & seed_name,
                     bool const on);

private:
    T2Tokens & tokens;
};

} // end namespace
#endif // _MZN_TOKENS_MANAGER_H_
