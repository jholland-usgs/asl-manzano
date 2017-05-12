#include "sensor.h"
#include "json_cals.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void Sensor::stream_config(std::ostream & os) const {

    os << "\n    " << "input    : " << config.input
       << "\n    " << "model    : " << config.model
       << "\n    " << "cals     : " << config.cals
       << std::boolalpha
       << "\n    " << "has_e300 : " << config.has_e300
       << std::noboolalpha;

    if (config.has_e300) os << port_e300_const_ref();

    os << "\n\n --- cal sequence ---";
    auto const cals_json = Utility::get_cals_json(config.cals);
    auto const cals_lines = Utility::cals_lines(cals_json);

    for (unsigned i = 0; i < cals_lines.size(); i++) {
        os << "\n " << i << ": " << cals_lines[i];
    }
}

// -------------------------------------------------------------------------- //
void Sensor::stream_status(std::ostream & os) const {

    if (config.has_e300) {
        os << "E300:";
        os << std::boolalpha
           << "\n    " << "connected  : " << port_e300_const_ref().connected()
           << "\n    " << "registered : " << port_e300_const_ref().registered;
    }
}

} // <- mzn
