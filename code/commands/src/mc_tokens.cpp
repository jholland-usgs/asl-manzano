#include "t1_comm_event.h"
#include "t1_irr_filter.h"

namespace mzn {

// this is called after base_set_data
// the header data is available

// number of bytes (nb) includes the bytes of itself
// here we are interested in the bytes after nb, which was
// taken care of in base_set_data

// -------------------------------------------------------------------------- //
uint16_t token_header_size(uint8_t const cmd_number) {

    std::bitset<8> const cmd_number_bitset(cmd_number);
    // from dp token definitions manual, bit 6 defines this variable
    bool const header_is_2_bytes = cmd_number_bitset.test(6);

    if (header_is_2_bytes) return 2;
    else return 1;
}

// -------------------------------------------------------------------------- //
uint16_t T1CommEvent::nb(std::vector<uint8_t> const & msg, uint16_t mf_begin) const {
    // TODO this works for event, but need more general for bigger headers
    auto const header_size = token_header_size( this->cmd_number() );
    auto const N = this->number_of_bytes() - header_size;
    return N;
}

// -------------------------------------------------------------------------- //
uint16_t T1IrrFilter::ni(std::vector<uint8_t> const & msg, uint16_t mf_begin) const {

    return this->number_of_sections();
}

}
