#include "cx_environmental_processor_status.h"

namespace mzn {
uint16_t CxEnvironmentalProcessorStatus::number_of_ic(std::vector<uint8_t> const & msg,
                                                      uint16_t mf_begin) const {
    // the header data is available
   return 2;

}
}
