// ** Comm Object ** //
// Manzano software
#include "comm.h"
namespace mzn {

// -------------------------------------------------------------------------- //
/*
inline
std::chrono::time_point<Time::SysClock, Time::Seconds<> >
q_time(TargetAddress const & ta) {

    // request status
    C1Rqstat cmd_rqstat;
    cmd_rqstat.request_bitmap.global_status(true);
    // status
    C1Stat cmd_stat; // Status

    md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);

    CxGlobalStatus * gs =
        dynamic_cast<CxGlobalStatus *>( cmd_stat.inner_commands[0].get() );

    if (gs == nullptr) throw FatalException("Comm", "autocal", "gs nullptr");

    // Q330 manual: "Seconds offset ... when added to a data sequence number
    // is seconds since January 1 2000"
    auto const q_data_seq_number = gs -> data_sequence_number.data();
    auto const q_seconds_offset = gs -> seconds_offset.data();
    auto const q_sec_since_epoch = q_data_seq_number + q_seconds_offset;

    CmdFieldTime<uint32_t, Time::k_shift_seconds_1970_2000> q_now_time;
    q_now_time.data(q_sec_since_epoch);

    std::cout << std::endl << "q_now_seq_num: " << q_sec_since_epoch;
    std::cout << std::endl << "q_now_time: " << q_now_time;
    std::cout << std::endl << " ### now: "
                      << Time::sys_time_of_day() << " ###\n";

    return q_now_time();

    return std::chrono::time_point< Time::SysClock, Time::Seconds<> >{};
}

*/
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

