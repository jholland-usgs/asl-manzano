// ** command line interface ** //
// Manzano Software //
#include <cstdlib>
#include "mcew_connection.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void McewConnection::replace(std::string const & user_input) const {

    // check basic format
    auto station_names = Utility::get_tokens(user_input, ' ');
    Utility::capitalize_tokens(station_names);

    if ( station_names.empty() ) {
        throw WarningException("McewConnection",
                               "pull_config",
                               "no stations provided");
    }

    auto const temp_file_path = config_home_path + "/config.tmp";
    auto const config_file_path = config_home_path + "/config.json";

    // clear any old temp file before opening a new one
    // ---------------------------------------------------------------------- //
    {
        std::ofstream ofs;
        ofs.open(temp_file_path, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }

    // run script
    // ---------------------------------------------------------------------- //
    // this bash script calls curl, its configuration is expected in the same
    // place as the config_file_path. The binary same as mce.
    auto curl_cmd = std::string("bash get_mzn_config.sh -o ") + temp_file_path;

    for (auto const & station_name : station_names) {
        curl_cmd += std::string(" -a ") + station_name;
    }

    Utility::system_call(curl_cmd);

    // check the temp file is not empty
    // ---------------------------------------------------------------------- //
    {
        std::ifstream ifs;
        ifs.open(temp_file_path);

        bool const ifs_is_empty =
            ( ifs.peek() == std::ifstream::traits_type::eof() );

        if ( (not ifs) or ifs_is_empty) {
            std::cout << std::endl;
            throw WarningException("McewConnection",
                                   "pull_config",
                                   "can't open temp config file");
        }
        std::cout << std::endl << "__RAW FILE FROM SERVER__\n" << ifs.rdbuf();
    }

    // test if configuration is valid
    // --------------------------------------------------------- //
    // test if the file can be made into a seismic network
    SeismicNetwork const sn_temp(temp_file_path);
    // ok, it created some valid seismic network.
    // now, check if the stations on this sn_temp are the same as desired
    auto sn_has_st = [&sn_temp](std::string const & station_name) {
        for (auto const & st : sn_temp.st) {
            if (st.config.station_name == station_name) return true;
        }
        return false;
    };

    for (auto const & station_name : station_names) {
        if ( not sn_has_st(station_name) ) {
            auto const msg_error = std::string("station not found:[") +
                                   station_name + std::string("]");
            throw WarningException("McewConnection", "pull_config", msg_error);
        }
    }

    // all good, replace current config with temp file
    auto const mv_cmd = std::string("mv ") + temp_file_path +
                        std::string(" ") + config_file_path;

    Utility::system_call(mv_cmd);
}

// -------------------------------------------------------------------------- //
void McewConnection::add_to_config(SeismicNetwork const & sn,
                                  std::string const & user_input) const {
    // repeated stations are ok
    std::stringstream ss;
    for (auto const & st : sn.st) ss << " " << st.config.station_name;
    auto const appended_user_input = user_input + ss.str();
    change_config(appended_user_input);
}

} // end namespace
