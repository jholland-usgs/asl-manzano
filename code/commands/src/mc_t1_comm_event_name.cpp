#include "t1_comm_event_name.h"
namespace mzn {

uint16_t T1CommEventName::number_of_ic(std::vector<uint8_t> const & msg,
                                       uint16_t mf_begin) const {
    // this is called after base_set_data
    // the header data is available

    // number_of_bytes (nob) includes the bytes of itself
    // here we are interested in the bytes after nob, which was
    // taken care of in base_set_data
    // nob is a 2 byte field for this token
    auto const N = this->number_of_bytes() - 2;

    // command_container is based on number of entries
    // .. as the rest of the digitizer.. but the tokens are based on
    // number of bytes to tell about the bytes to come.
    // command_container handling is the default case, this will be a special
    // case for tokens. In this case, it would be to read the msg and count
    // the number of inner commands (nic).
    int nic = 0;

    for (auto i = mf_begin; i < mf_begin + N; ) {
        i++; // event number
        uint8_t const N_pstring = msg[i] + 1;
        i += N_pstring; // event name
        nic++;
    }

    return nic;
}

}
