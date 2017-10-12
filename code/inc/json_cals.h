#ifndef _MZN_JSON_CALS_H_
#define _MZN_JSON_CALS_H_

#include "json_mzn.h"

namespace mzn {
namespace Utility {

// -------------------------------------------------------------------------- //
inline
std::string cal_line(Json const & cal_json) {

    std::stringstream ss;

    ss << cal_json["wave_form"] << "\t"
       << cal_json["amplitude"] << "db" << "\t"
       << cal_json["cal_duration"]["h"] << "h_"
       << cal_json["cal_duration"]["m"] << "m" << "\t";

    if (cal_json["wave_form"] != "step") ss << cal_json["frequency"] << "hz";

    return ss.str();
}

// -------------------------------------------------------------------------- //
inline
std::vector<std::string> cals_lines(Json const & cals_json) {
    std::vector<std::string> lines;
    for (auto const & cal_json : cals_json) lines.push_back( cal_line(cal_json) );
    return lines;
}

// -------------------------------------------------------------------------- //
inline
void check_cal_json(Json const & cal) {

    // give a bit more specific warning message than the default on JsonRef
    auto cal_field_check = [](std::string const & cal_field,
                              JsonRef const check_json) {

        if ( not check_json.has(cal_field) ) {
            std::stringstream ss;
            ss << "calibration does not have field:(" << cal_field;
            throw WarningException( "Utility",
                                    "check_cal_json",
                                    ss.str() );
        }
    };

    cal_field_check("wave_form", cal);
    cal_field_check("amplitude", cal);
    cal_field_check("cal_duration", cal);
    cal_field_check("settling_time", cal);
    cal_field_check("trailer_time", cal);

    auto const waveform_json = cal["wave_form"];
    if (waveform_json != "step") cal_field_check("frequency", cal);

    cal_field_check("h", cal["cal_duration"]);
    cal_field_check("m", cal["cal_duration"]);
    cal_field_check("h", cal["settling_time"]);
    cal_field_check("m", cal["settling_time"]);
    cal_field_check("h", cal["trailer_time"]);
    cal_field_check("m", cal["trailer_time"]);
}

//! return json with the cal sequence for a specific sensor
//! cal_key is typically the same as sensor model
// -------------------------------------------------------------------------- //
inline
Json get_cals_json(std::string const & cal_key) {

    auto const runtime_config_path = get_runtime_config_path();
    auto const cal_file_path = runtime_config_path +
                               std::string("/cal_sequences.json");

    auto const cals_json = Utility::read_json(cal_file_path);
    auto const cals_json_ref = JsonRef(cals_json);

    try {

        auto const sensor_cals_json = cals_json_ref[cal_key];
        auto const sensor_cals_json_ref = JsonRef(sensor_cals_json);
        auto const array_cals_json = sensor_cals_json_ref["cals"];
        // check for the right format
        for (auto const & cal_json : array_cals_json) check_cal_json(cal_json);
        // return specifically the array of cals
        return array_cals_json;

    } catch (Exception const & e) {

        std::cerr << e.what();

        std::stringstream ss;
        ss << "sensor (" << cal_key << ") not found in cal sequences file";
        throw WarningException( "Utility",
                                "get_cals",
                                 ss.str() );
    }
}

// -------------------------------------------------------------------------- //
inline
int max_port_host() {
    auto const runtime_config_path = get_runtime_config_path();
    auto const file_path = runtime_config_path + std::string("/config.json");
    auto const j = Utility::read_json(file_path);
    int max_ = 0;
    for (auto const & st_json : j["station"]) {
        for (auto const & q_json : st_json["digitizer"]) {
             int const & port_host = q_json["port_config"]["port_host"];
             if (port_host > max_) max_ = port_host;
        }
    }
    return max_;
}
} // <- Utility
} // <- mzn

#endif
