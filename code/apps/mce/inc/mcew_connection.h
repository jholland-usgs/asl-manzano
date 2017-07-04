// ** command line interface ** //
// Manzano Software //
#ifndef _MZN_MCEW_CONNECTION_H_
#define _MZN_MCEW_CONNECTION_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include "mzn_except.h"
#include "system_calls.h"
#include "user_interpreter.h"
#include "instruction_interpreter.h"
#include "json_sn.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class McewConnection {
public:

    McewConnection() = default;
    ~McewConnection() = default;

    std::string config_dir_path;
    std::string config_file_path;
    std::string const temp_config_file_name = "config.tmp";

    using StNms = std::vector<std::string>;

    // ---------------------------------------------------------------------- //
    bool sn_has_st(SeismicNetwork const & sn,
                   std::string const & station_name) const;

    bool sn_has_st(Json const & sn_json,
                   std::string const & station_name) const;

    // ---------------------------------------------------------------------- //
    Json fetch_stations(StNms station_names) const;

    // ---------------------------------------------------------------------- //
    void update_all(SeismicNetwork const & sn) const;
    void update(SeismicNetwork const & sn, StNms const & station_names) const;
    void use(SeismicNetwork const & sn, StNms const & station_names) const;
    void get(SeismicNetwork const & sn, StNms const & station_names) const;
    void send(SeismicNetwork const & sn, StNms const & station_names) const;

};

} // end namespace
#endif
