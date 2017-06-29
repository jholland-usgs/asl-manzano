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

    std::string config_home_path;

    void get_config(SeismicNetwork const & sn,
                    std::string const & user_input = "") const;

    void change_config(std::string const & user_input) const;

    void get_config(SeismicNetwork const & sn,
                    std::string const & user_input = "") const;

    std::vector<Station> fetch_stations(std::vector<std::string> const station_names);

private:

    TargetAddress ta_;

};

} // end namespace
#endif
