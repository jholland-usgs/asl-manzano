#include "cx_gps_satellites.h"

namespace mzn {

// -------------------------------------------------------------------------- //
uint16_t CxGpsSatellites::ni(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) const {

    // this is called after base_set_data
    // the header data is available
   return this->number_of_entries();

}
}
