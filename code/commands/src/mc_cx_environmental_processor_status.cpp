#include "cx_environmental_processor_status.h"

namespace mzn {

// -------------------------------------------------------------------------- //
uint16_t CxEnvironmentalProcessorStatus::ni(std::vector<uint8_t> const & msg,
                                            uint16_t mf_begin) const {
    // the header data is available
   return 2;

}
}
