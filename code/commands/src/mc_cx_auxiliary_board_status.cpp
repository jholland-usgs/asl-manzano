#include "cx_auxiliary_board_status.h"

namespace mzn {

// -------------------------------------------------------------------------- //
uint16_t CxAuxiliaryBoardStatus::ni(std::vector<uint8_t> const & msg,
                                    uint16_t mf_begin) const {
    // this is called after base_set_data
    // the header data is available

   return (size_of_this_block.data() - 8)/4;

}
}
