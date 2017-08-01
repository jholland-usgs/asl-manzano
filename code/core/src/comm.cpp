// ** Comm Object ** //
// Manzano software
#include "comm.h"
namespace mzn {

// -------------------------------------------------------------------------- //
Comm::Comm() : sn{},
               md{},
               output_store{},
               input_store{sn},
               stream_output{sn},
               ip_address_number{0},
               msg_task_manager_{sn, md},
               cmd_file_reader_{sn} {}

} // << mzn

