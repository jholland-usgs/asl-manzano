#include "t1_comm_event_name.h"
namespace mzn {

int T1CommEventName::number_of_ic(std::size_t const msg_size) const {
    // this is called after base_set_data
    // the header data is available

    auto const size = this->number_of_bytes();

    // multi_command is based on number of entries
    // .. as the rest of the digitizer.. but the tokens are based on
    // number of bytes to tell about the bytes to come.
    // multi_command handling is the default case, this will be a special
    // case for tokens. In this case, it would be to read the msg and count
    // the number of inner commands. Can't because I can't read the msg.


    return size;
}

}
