#include "t1_comm_event.h"
#include "t1_irr_filter.h"
#include "t1_fir_filter.h"
#include "t2_tokens.h"
#include "c1_mem.h"

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
uint16_t C1Mem::nb(std::vector<uint8_t> const & msg, uint16_t mf_begin) const {
    auto const N = this->byte_count() - this->cmd_data_size();
    return N;
}

// -------------------------------------------------------------------------- //
uint16_t T1FirFilter::nb(std::vector<uint8_t> const & msg, uint16_t mf_begin) const {
    // TODO this works for fir filter, but need more general for bigger headers
    auto const header_size = token_header_size( this->cmd_number() );
    auto const N = this->number_of_bytes() - header_size;
    return N;
}

// -------------------------------------------------------------------------- //
uint16_t T1ControlDetectorSpecification::nb(std::vector<uint8_t> const & msg,
                                            uint16_t mf_begin) const {
    // specific to this one
    auto const header_size = token_header_size( this->cmd_number() );
    auto constexpr other_vars_size = 2;
    auto const detector_name_size = this->detector_name().size();

    auto const total_extra_size =
        header_size + other_vars_size + detector_name_size;

    auto const N = this->number_of_bytes() - total_extra_size;

    return N;
}
// -------------------------------------------------------------------------- //
uint16_t T1IrrFilter::ni(std::vector<uint8_t> const & msg, uint16_t mf_begin) const {

    return this->number_of_sections();
}

// -------------------------------------------------------------------------- //
bool T1LogicalChannelQueue::command_active(uint8_t const cmd_key) const {
   return option.data_.test(cmd_key);
}

// -------------------------------------------------------------------------- //
uint16_t T2Tokens::nb(std::vector<uint8_t> const & msg, uint16_t mf_begin) const {
    // tokens does not have any header. Only works for the whole msg
    auto const N = msg.size();
    return N;
}

}
