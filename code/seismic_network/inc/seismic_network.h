// ** SeismicNetwork Object ** //
// Manzano software

#ifndef _MZN_SEISMIC_NETWORK_H
#define _MZN_SEISMIC_NETWORK_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "station.h"
#include "target_address.h"
#include "mzn_except.h"
#include "json.h"

namespace mzn {

//! Performs the configuration of the entire seismic network
/*!
    holds the stations
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class SeismicNetwork {

public:

    // --------------------------------------------------------------------- //
    struct Config {

        friend class SeismicNetwork;
        Config() = default;
        ~Config() = default;
    };

    struct Status {
        friend class SeismicNetwork;
    };

    /*! reads the json file and configures stations, data processors, digitizers
        if successful, sets the configured flag to true
        it adds to the vector st
     */
    // --------------------------------------------------------------------- //
    SeismicNetwork();
    SeismicNetwork(std::string const & file_path);
    SeismicNetwork(Json const & sn_json);
    ~SeismicNetwork() = default;

    //! holds a vector of stations
    std::vector<Station> st; //stations
    //! flag sets to true during constructor initialization

    // --------------------------------------------------------------------- //
    Config config;
    Status status;

    // --------------------------------------------------------------------- //
    Digitizer & q_ref(TargetAddress const & ta);
    Sensor & s_ref(TargetAddress const & ta);
    Station & st_ref(TargetAddress const & ta);
    DataProcessor & dp_ref(TargetAddress const & ta) ;

    // --------------------------------------------------------------------- //
    Digitizer const & q_const_ref(TargetAddress const & ta) const;
    Sensor const & s_const_ref(TargetAddress const & ta) const;
    Station const & st_const_ref(TargetAddress const & ta) const;
    DataProcessor const & dp_const_ref(TargetAddress const & ta) const;

    // --------------------------------------------------------------------- //
    Station const & st_const_ref(std::string const & station_name) const;

    // --------------------------------------------------------------------- //
    bool has_station(std::string const & station_name) const {
        for (unsigned i = 0; i < st.size(); i++) {
            if (station_name == st[i].config.station_name) return true;
        }
        return false;
    }

    // --------------------------------------------------------------------- //
    unsigned station_index(std::string const & station_name) const {
        for (unsigned i = 0; i < st.size(); i++) {
            if (station_name == st[i].config.station_name) return i;
        }
        std::stringstream ss;
        ss << "station ["  << station_name << "] not found";
        throw WarningException( "SeismicNetwork",
                                "station_index",
                                ss.str() );
    }

    // ---------------------------------------------------------------------- //
    void stream_config(std::ostream & os) const;
    void stream_status(std::ostream & os) const;
};

} // << mzn
#endif // _MZN_SEISMIC_NETWORK_H_

