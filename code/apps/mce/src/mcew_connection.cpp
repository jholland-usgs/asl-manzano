// ** command line interface ** //
// Manzano Software //
#include <cstdlib>
#include "mcew_connection.h"

namespace mzn {

// -------------------------------------------------------------------------- //
bool McewConnection::sn_has_st(SeismicNetwork const & sn,
                               std::string const & station_name) const {

    for (auto const & st : sn.st) {
        if (st.config.station_name == station_name) return true;
    }
    return false;
};

// -------------------------------------------------------------------------- //
bool McewConnection::sn_has_st(Json const & sn_json,
                               std::string const & station_name) const {

    SeismicNetwork const sn_temp(sn_json);
    return sn_has_st(sn_temp, station_name);
}

// -------------------------------------------------------------------------- //
void McewConnection::update_all(SeismicNetwork const & sn) const {
    StNms station_names;
    for (auto const & st : sn.st) station_names.push_back(st.config.station_name);
    auto updated_sn_json = fetch_stations(station_names);
    Utility::save_to_config_file(updated_sn_json, config_file_path);
}

// -------------------------------------------------------------------------- //
void McewConnection::use(SeismicNetwork const & sn,
                         StNms const & station_names) const {

    auto updated_sn_json = fetch_stations(station_names);
    Utility::save_to_config_file(updated_sn_json, config_file_path);
}

// -------------------------------------------------------------------------- //
void McewConnection::update(SeismicNetwork const & sn,
                            StNms const & station_names) const {

    for (auto const & station_name : station_names) {
        if ( not sn_has_st(sn, station_name) ) {
            std::stringstream ss;
            ss << "station [" << station_name << "] could not be updated\n"
               << "because it is not in the current seismic network\n";
            throw WarningException("McewConnection", "update", ss.str() );
        }
    }

    auto const current_sn_json = Utility::json_from_sn(sn);
    auto const fetched_sn_json = fetch_stations(station_names);
    SeismicNetwork fetched_sn(fetched_sn_json);

    auto updated_sn_json = Utility::empty_sn_json();

    // some extra work to keep the same order as the current sn
    for (auto const & st: sn.st) {
        auto const & station_name = st.config.station_name;
        auto const fetched_version_available = sn_has_st(fetched_sn,
                                                         station_name);
        if (fetched_version_available) {
            auto const & st_fetched = fetched_sn.st_const_ref(station_name);
            auto const st_fetched_json = Utility::json_from_st(st_fetched);
            updated_sn_json["station"].push_back(st_fetched_json);
        } else {
            auto const st_current_json = Utility::json_from_st(st);
            updated_sn_json["station"].push_back(st_current_json);
        }
    }

    Utility::save_to_config_file(updated_sn_json, config_file_path);
}

// -------------------------------------------------------------------------- //
void McewConnection::get(SeismicNetwork const & sn,
                         StNms const & station_names) const {

    // all stations in station_names must not exist in the sn
    auto const current_sn_json = Utility::json_from_sn(sn);
    for (auto const & station_name : station_names) {
        if ( sn_has_st(current_sn_json, station_name) ) {
            std::stringstream ss;
            ss << "station [" << station_name << "] is already in the\n"
               << "current seismic network\n";
            throw WarningException("McewConnection", "get", ss.str() );
        }
    }

    auto updated_sn_json = fetch_stations(station_names);

    for (auto const & station_json : current_sn_json["station"]) {
        updated_sn_json["station"].push_back(station_json);
    }

    Utility::save_to_config_file(updated_sn_json, config_file_path);
}

// -------------------------------------------------------------------------- //
void McewConnection::send(SeismicNetwork const & sn,
                          StNms const & station_names) const {

}

// -------------------------------------------------------------------------- //
Json McewConnection::fetch_stations(StNms station_names) const {

    if ( station_names.empty() ) return Utility::empty_sn_json();

    Utility::capitalize_tokens(station_names);

    auto const temp_file_path = config_dir_path + "/" + temp_config_file_name;

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
                                   "fetch_stations",
                                   "can't open temp config file");
        }
        std::cout << std::endl << "__RAW FILE FROM SERVER__\n" << ifs.rdbuf();
    }

    // test if configuration is valid
    // --------------------------------------------------------- //
    // test if the file can be made into a seismic network
    SeismicNetwork sn_temp(temp_file_path);

    // check if the stations on this sn_temp are the same as desired
    // --------------------------------------------------------- //
    for (auto const & station_name : station_names) {
        if ( not sn_has_st(sn_temp, station_name) ) {
            auto const msg_error = std::string("station not found:[") +
                                   station_name + std::string("]");
            throw WarningException("McewConnection", "fetch_config", msg_error);
        }
    }

    // all good
    // --------------------------------------------------------- //
    auto const sn_json = Utility::read_json(temp_file_path);
    return sn_json;
}

} // end namespace
