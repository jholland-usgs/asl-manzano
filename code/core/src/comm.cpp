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

// -------------------------------------------------------------------------- //
std::unique_ptr<T2Tokens> Comm::pop_tokens(TargetAddress const & ta) {
    if (not output_store.contains<Action::get, Kind::token>(ta)) {
        OptionInput const oi_data_port("1");
        run<Action::get, Kind::token>(ta, oi_data_port);
    }
    std::unique_ptr<T2Tokens> tokens_ptr =
        output_store.pop_output_cmd<Action::get, Kind::token>(ta);
    return tokens_ptr;
}

// -------------------------------------------------------------------------- //
void Comm::save_tokens(std::unique_ptr<T2Tokens> & tokens_ptr,
                       TargetAddress const & ta) {
    auto constexpr ui_id = UserInstruction::hash(Action::get, Kind::token);
    auto const task_id = ta.hash() + ui_id;
    // move to cmd store
    output_store.cmd_map[task_id] = std::move(tokens_ptr);
    tokens_ptr.reset();
}

} // << mzn

