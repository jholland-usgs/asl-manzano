// ** Comm Object ** //
// Manzano software
#ifndef _MZN_COMM_H
#define _MZN_COMM_H

#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <random>
#include "cmd_file_reader.h"
#include "msg_task_manager.h"
#include "output_store.h"
#include "input_store.h"
#include "stream_output.h"
#include "stream_plotter.h"
#include "message_dispatch.h"
#include "string_utilities.h"
#include "system_calls.h"
#include "tokens_manager.h"
// external libraries
#include "md5.h" // jason holland's (usgs) md5 library
#include "date.h" // hinnant's date library

namespace mzn {
//! Commands are setup and send/recv from this class
/*! @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class Comm {

public:
    //! autocal scheduler takes a ref to md_
    Comm();
    ~Comm() = default;

public:
    //! holds config, status and connections to hardware targets
    SeismicNetwork sn;
    //! takes cmd and send/recv a msg
    MessageDispatch md;
    //! holds the outputs of the send/recv commands
    OutputStore output_store;
    //! has functions to setup input commands
    /*! either creating an acceptable default, using a "shortcut option"
        or using a "live option" which takes a user created command
        @throws if an option is provided and not found in the shortcuts
        or in the custom made commands
        @see InputStore for more info on default/shortcut/live options
      */
    InputStore input_store;
    //! streams output to console
    StreamOutput stream_output;
    //! holds this computer ip as seen by digitizer registrations
    std::atomic<uint32_t> ip_address_number;

private:
    //! manages multi-threaded msg task that are waiting to be sent
    MsgTaskManager msg_task_manager_;
    //! uses threads to setup timed send of qcal cmds
    CmdFileReader cmd_file_reader_;

public:
    //! primary template
    //! a template specialization for each legal combination is provided
    //! can use typename std::enable_if<action != Action::get>::type
    // --------------------------------------------------------------------- //
    template <Action action, Kind kind>
    inline
    void run(TargetAddress const & ta,
             OptionInput const & oi = OptionInput{}) {
        std::stringstream ss;
        ss << "Comm::run general case is not defined"
           << "\nAction: " << action << " Kind: " << kind;
        throw std::logic_error{ss.str()};
    }

private:

    //! for use with a q. send a requests, gets a response
    //! @see Digitizer (q)
    // --------------------------------------------------------------------- //
    template <Action action, Kind kind>
    inline
    void q_send_recv(TargetAddress const & ta,
                     OptionInput const & oi = OptionInput{}) {

        // send and receive commands
        auto cmd_input = input_store.get_input_cmd<action, kind>(ta, oi);

        // empty (default constructed) cmd for receiving
        using Co = typename Co<action, kind>::type;
        Co cmd_output{};

        auto & q = sn.q_ref(ta);
        md.send_recv(q.port_config, cmd_input, cmd_output, true);

        auto constexpr ui_id = UserInstruction::hash(action, kind);
        auto const task_id = ta.hash() + ui_id;
        // Co needs to be move_constructible
        output_store.cmd_map[task_id] =
            std::make_unique<Co>( std::move(cmd_output) );
    }

    // --------------------------------------------------------------------- //
    bool q_is_reg(Digitizer & q) {
        C2Regresp cmd_regresp;
        return q_is_reg(q, cmd_regresp);
    }

    // --------------------------------------------------------------------- //
    bool q_is_reg(Digitizer & q, C2Regresp & cmd_regresp) {

        if (ip_address_number == 0) {
            cmd_regresp.registered(false);
            return false;
        }

        C2Regchk cmd_regchk;
        cmd_regchk.ip_address(ip_address_number);

        // turns out you can't ask if registered after de-registering
        // maybe because it had a specific purpose related to the balers
        // it should be used internally sparingly, in auto_ instructions only
        // when it doesn't work, the digitizer sends a C1Cerr message which
        // provides the same function, although using error handling

        try {

            md.send_recv(q.port_config, cmd_regchk, cmd_regresp, false);

        } catch (WarningException const & e) {

            std::string const e_msg = e.what();

            if (e_msg.find("you_are_not_registered") != std::string::npos) {
                // C1Cerr with you_are_not_registered code
                cmd_regresp.registered(false);
                q.port_config.registered = false;
                return false;

            } else throw e;
        }

        auto const reg_status = cmd_regresp.registered();
        q.port_config.registered = reg_status;
        return reg_status;
    }

    // --------------------------------------------------------------------- //
    uint32_t q_current_seq_number(Digitizer & q) {

        // request status
        C1Rqstat cmd_rqstat;
        cmd_rqstat.request_bitmap.global_status(true);
        // status
        C1Stat cmd_stat; // Status

        md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);

        CxGlobalStatus * gs =
            dynamic_cast<CxGlobalStatus *>( cmd_stat.inner_commands[0].get() );

        if (gs == nullptr) throw FatalException("Comm",
                                                "q_current_seq_number",
                                                "gs nullptr");

        return gs -> current_data_sequence_number();
    }

    // ---------------------------------------------------------------------- //
    CmdFieldTime<uint32_t, Time::k_shift_seconds_1970_2000>
    q_time(Digitizer & q) {

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
        auto const q_seq_number = gs -> current_data_sequence_number.data();
        auto const q_seconds_offset = gs -> seconds_offset.data();
        auto const q_sec_since_epoch = q_seq_number + q_seconds_offset;

        CmdFieldTime<uint32_t, Time::k_shift_seconds_1970_2000> q_now_time;
        q_now_time.data(q_sec_since_epoch);

        std::cout << std::endl << "q_now_seq_num: " << q_sec_since_epoch;
        std::cout << std::endl << "q_now_time: " << q_now_time;
        std::cout << std::endl << " ### now: "
                          << Time::sys_time_of_day() << " ###\n";

        return q_now_time;
    }

    //! These two functions can be generalized for any of the 14 output
    //! definitions. cal/center are currently used. In general, more than
    //! one output can be set in the sce, but this is not needed or tested.
    // --------------------------------------------------------------------- //
    BmSensorControlEnable sensor_control_cal(Digitizer & q,
                                             Sensor const & s) {
        // sensor_control_map, C1Rqsc , C1Sc
        C1Rqsc cmd_rqsc;
        C1Sc cmd_sc;
        md.send_recv(q.port_config, cmd_rqsc, cmd_sc, false);

        auto const sco = (s.config.input == Sensor::Input::a) ?
                         BmSensorControlOutput::Line::sensor_a_calibration :
                         BmSensorControlOutput::Line::sensor_b_calibration;

        BmSensorControlEnable sce;

        if (cmd_sc.sensor_output_1a.line() == sco) sce.output_1a(true); else
        if (cmd_sc.sensor_output_2a.line() == sco) sce.output_2a(true); else
        if (cmd_sc.sensor_output_3a.line() == sco) sce.output_3a(true); else
        if (cmd_sc.sensor_output_4a.line() == sco) sce.output_4a(true); else
        if (cmd_sc.sensor_output_1b.line() == sco) sce.output_1b(true); else
        if (cmd_sc.sensor_output_2b.line() == sco) sce.output_2b(true); else
        if (cmd_sc.sensor_output_3b.line() == sco) sce.output_3b(true); else
        if (cmd_sc.sensor_output_4b.line() == sco) sce.output_4b(true); else
        throw WarningException("Comm",
                               "sensor_control_cal",
                               "No sensor control configured for calibration");
        return sce;
    }

    // --------------------------------------------------------------------- //
    BmSensorControlEnable sensor_control_center(Digitizer & q,
                                                Sensor const & s) {
        // sensor_control_map, C1Rqsc , C1Sc
        C1Rqsc cmd_rqsc;
        C1Sc cmd_sc;
        md.send_recv(q.port_config, cmd_rqsc, cmd_sc, false);

        auto const sco = (s.config.input == Sensor::Input::a) ?
                         BmSensorControlOutput::Line::sensor_a_centering :
                         BmSensorControlOutput::Line::sensor_b_centering;

        BmSensorControlEnable sce;

        if (cmd_sc.sensor_output_1a.line() == sco) sce.output_1a(true); else
        if (cmd_sc.sensor_output_2a.line() == sco) sce.output_2a(true); else
        if (cmd_sc.sensor_output_3a.line() == sco) sce.output_3a(true); else
        if (cmd_sc.sensor_output_4a.line() == sco) sce.output_4a(true); else
        if (cmd_sc.sensor_output_1b.line() == sco) sce.output_1b(true); else
        if (cmd_sc.sensor_output_2b.line() == sco) sce.output_2b(true); else
        if (cmd_sc.sensor_output_3b.line() == sco) sce.output_3b(true); else
        if (cmd_sc.sensor_output_4b.line() == sco) sce.output_4b(true); else
        throw WarningException("Comm",
                               "sensor_control_center",
                               "No sensor control configured for centering");
        return sce;
    }

    // ---------------------------------------------------------------------- //
    template <typename Point>
    inline
    Point boom_positions(Digitizer & q, Sensor const & s) {
        // request status
        C1Rqstat cmd_rqstat;
        cmd_rqstat.request_bitmap.boom_positions(true);
        // status
        C1Stat cmd_stat;
        // needs to be registered
        md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);
        // get global status
        CxBoomPositions * bp =
            dynamic_cast<CxBoomPositions *>( cmd_stat.inner_commands[0].get() );

        if (bp == nullptr) throw FatalException("Comm",
                                                "boom_positions",
                                                "boom positions nullptr");

        if (s.config.input == Sensor::Input::a) {

            return Point { bp -> channel_1_boom(),
                           bp -> channel_2_boom(),
                           bp -> channel_3_boom() };
        } else {

            return Point { bp -> channel_4_boom(),
                           bp -> channel_5_boom(),
                           bp -> channel_6_boom() };
        }
    }

    /*
    // --------------------------------------------------------------------- //
    template <class Cs, Action action, Kind kind>
    inline
    void cmd_show_options() {

        stream_output.os << "\nshortcut options:";
        stream_output.show_tree( InstructionMap::filter_options<kind>() );

        auto const live_options = input_store.live_options<action, kind>();

        stream_output.os << "\n\nlive options:";
        stream_output.show_num_tree(live_options);

        //TODO: change when the up arrow is added in the user input loop
        unsigned int live_option_index;
        std::string live_option_token;

        // only std::cin makes sense here, no need for indirection
        try {
            std::getline(std::cin, live_option_token);
            live_option_index = std::stoi(live_option_token);

        } catch(std::exception & e) {
            throw WarningException( "Comm",
                                    "cmd_show_options",
                                    "Bad selection provided\n" +
                                    std::string( e.what() ) );
        }

        // user selected "cancel"
        if (live_option_index == live_options.size() + 1) {
            stream_output.os << "\n cancelled";
            return;
        }

        if ( live_option_index == live_options.size() ) {
            stream_output.os << "\n option name: ";
            std::string option;
            std::getline(std::cin, option);
            std::cout << std::endl << "option chosen " << option << std::endl;
            input_store.add_live_cmd<Cs>(option);
            return;
        }

        // user selected an existing live command
    }

    */

    /*
    //! to autocal_scheduler_.autocal_start
    void autocal_start(Digitizer & q, Sensor const & s);
    */
};

// -------------------------------------------------------------------------- //
//TODO: remove from namespace
using TA = TargetAddress;
using OI = OptionInput;

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::poll>(TA const & ta, OI const & oi) {
    // C1Pollsn, C1Mysn
    q_send_recv<Action::get, Kind::poll>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::ping>(TA const & ta, OI const & oi) {
    // C1Ping4, C1Ping5
    q_send_recv<Action::get, Kind::ping>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::global>(TA const & ta, OI const & oi) {
    // C1Rqglob, C1glob
    q_send_recv<Action::get, Kind::global>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::stat>(TA const & ta,  OI const & oi) {
    // C1Rqstat, C1Stat (multi command)
    q_send_recv<Action::get, Kind::stat>(ta, oi);
    // TODO: make its own command
    auto & q = sn.q_ref(ta);
    auto const t = q_time(q);
    std::cout << std::endl << "time [" << t << "]";
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::center>(TA const & ta, OI const & oi) {
    // C2Rqamass, C2Amass
    q_send_recv<Action::get, Kind::center>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::output>(TA const & ta, OI const & oi) {
    // sensor_control_map, C1Rqsc , C1Sc
    q_send_recv<Action::get, Kind::output>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::dev>(TA const & ta, OI const & oi) {
    // devices, C1Rqdev, C1Dev (multicommand)
    q_send_recv<Action::get, Kind::dev>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::token>(TA const & ta,  OI const & oi) {
    // send and receive commands
    auto cmd_rqmem = input_store.get_input_cmd<Action::get, Kind::token>(ta, oi);
    uint16_t address = 0;
    auto & q = sn.q_ref(ta);
    std::vector<uint8_t> tokens_msg;

    while (true) {
        cmd_rqmem.starting_address(address);
        // header of output cmd. Combining several C1Mem we make one T2Tokens
        // cannot be done at once as each C1Mem needs to be requested separately
        // since its size is basically at the mtu (maximum transfer unit)
        C1Mem cmd_mem;
        md.send_recv(q.port_config, cmd_rqmem, cmd_mem, false);
        // one of the reasons to have CxMem separate from C1Mem
        if ( cmd_mem.inner_commands.empty() ) {
            throw InfoException("Comm", "get_token", "no token data on memory");
        }
        CxMem * mem = dynamic_cast<CxMem *>( cmd_mem.inner_commands[0].get() );
        if (mem == nullptr) throw FatalException("Comm", "get token", "mem nullptr");

        // the command includes information about size
        auto const mem_reported_size = cmd_mem.byte_count();
        auto const & mem_msg = mem->memory_contents.data();

        // size of segment_number + total_number_of_segments
        auto constexpr mem_header_size = 4u;
        auto const mem_size = mem_msg.size() + mem_header_size;
        if (mem_size != mem_reported_size) {
            throw FatalException("Comm",
                                 "get token",
                                 "mem size differs from reported");
        }
        // one big tokens_msg, from memory chunks
        // the chunks are incomplete, might cut in the middle of a token
        // the big tokens_msg does have entire tokens
        tokens_msg.insert( tokens_msg.end(), mem_msg.begin(), mem_msg.end() );
        // check if done with chunks
        auto const current = mem->segment_number;
        auto const limit = mem->total_number_of_segments;
        if (current == limit) break;
        // prepare next request
        auto constexpr full_chunk_size = 448u;
        address += full_chunk_size;
    }
    // stream and store output
    T2Tokens tokens;
    tokens.msg_to_data(tokens_msg, 0);
    std::cout << std::endl << tokens;
    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::get, Kind::token);
    auto const task_id = ta.hash() + ui_id;
    // move to cmd store
    output_store.cmd_map[task_id] = std::make_unique<T2Tokens>( std::move(tokens) );
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::set, Kind::token>(TA const & ta,  OI const & oi) {
    if (not output_store.contains<Action::get, Kind::token>(ta)) {
        OptionInput const oi_data_port("1");
        run<Action::get, Kind::token>(ta, oi_data_port);
    }
    // pop_output_cmd throws if not found. Can a digitizer start without any tokens?
    // if so, then the special case would need to be handled with an empty object
    std::unique_ptr<T2Tokens> const tokens_ptr =
        output_store.pop_output_cmd<Action::get, Kind::token>(ta);
    // TODO: do on its own function
    auto & tokens = *tokens_ptr;
    TokensManager tm{tokens};
    tm.set_hf_on();
    // else if (oi.option == "hfoff") tm.set_hf_off();
    //else throw WarningException("Comm", "run set token", "bad option");

    // there is no way to know the size at runtime just from looking at the cmd
    // in general it could be gotten when processing the input
    // the typical size is around 2000, 8000 is the maximum from the manual
    auto constexpr mem_size_max = 8000u;
    std::vector<uint8_t> tokens_msg(mem_size_max);
    // return of data_to_msg is the pos for the next cmd, but we are done
    auto const mem_size = tokens.data_to_msg(tokens_msg, 0);
    tokens_msg.resize(mem_size);
    // split into chunks and process
    // -----------------------------
    /*
    CmdFieldHex<uint32_t> starting_address;
    CmdField<uint16_t> byte_count;
    BmMemoryType memory_type;
    CmdField<uint16_t> segment_number;
    CmdField<uint16_t> total_number_of_segments;
    CmdFieldVector<0> memory_contents; */
    // the actual maximum msg size is 450 bytes, from which 448 bytes are
    // written to memory (not clear which 2 bytes are left out) or
    // maybe is written in a different way. It is confirmed that the msg
    // should have the 12 bytes of smem header + 438 bytes of actual token mem
    auto constexpr tkn_mem_max = 438u;

    auto const size_div = std::div(mem_size, tkn_mem_max);
    auto const number_of_chunks =
        (size_div.rem == 0) ? size_div.quot : size_div.quot + 1;
    auto & q = sn.q_ref(ta);
    uint16_t address = 0;
    for (auto i = 0u; i < number_of_chunks; i++) {
        bool const last_chunk = (i + 1 == number_of_chunks);
        auto const mem_chunk_size = last_chunk ? size_div.rem : tkn_mem_max;
        // C1Smem is implemented as a single command (no CxSmem) as there is
        // always a minimum token memory data to provide
        // only sets memory type according to option
        auto cmd_smem = input_store.get_input_cmd<Action::set, Kind::token>(ta, oi);
        cmd_smem.starting_address(address);
        // 4B = segment_number (2B) and total_number_of_segments (2B), max 442u
        cmd_smem.byte_count(mem_chunk_size + 4u);
        cmd_smem.segment_number(i + 1);
        cmd_smem.total_number_of_segments(number_of_chunks);
        auto const chunk_msg_begin = tokens_msg.begin() + tkn_mem_max * i;
        auto const chunk_msg_end = chunk_msg_begin + mem_chunk_size;
        std::vector<uint8_t> const chunk_msg(chunk_msg_begin, chunk_msg_end);
        cmd_smem.memory_contents(chunk_msg);
        // std::cout << std::endl << cmd_smem;
        // cmd_data_size does not include the memory size but data_to_msg
        // will resize the message on the CmdFieldVector data_to_msg
        /*
        std::vector<uint8_t> smem_msg( cmd_smem.cmd_data_size() );
        cmd_smem.data_to_msg(smem_msg, 0);
        std::cout << std::endl << "SMEM MSG:" << std::endl;
        Utility::stream_hex(smem_msg);
        */
        C1Cack cmd_recv;
        md.send_recv(q.port_config, cmd_smem, cmd_recv, true);
        auto constexpr full_chunk_size = 448u;
        address += full_chunk_size;
    }
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::stop, Kind::cal>(TA const & ta, OI const & oi) {
    // C1Stop, C1Cack
    q_send_recv<Action::stop, Kind::cal>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::reg>(TA const & ta, OI const & oi) {

    // get current reg status from q, C2Regchk, C2Regresp
    auto & q = sn.q_ref(ta);
    C2Regresp cmd_regresp;
    q_is_reg(q, cmd_regresp);
    std::cout << cmd_regresp;

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::get, Kind::reg);
    auto const task_id = ta.hash() + ui_id;
    // move to cmd store
    output_store.cmd_map[task_id] =
        std::make_unique<C2Regresp>( std::move(cmd_regresp) );
}

// *** CALIBRATIONS *** //
// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::start, Kind::cal>(TA const & ta, OI const & oi) {

    auto constexpr action = Action::start;
    auto constexpr kind = Kind::cal;

    auto & q = sn.q_ref(ta);
    auto & s = sn.s_ref(ta);

    // --------- E300 registration ----------- //
    if (s.config.has_e300) {
        s.port_e300_ref().reg();
        std::cout << std::endl << "\n** E300 REGISTERED! ** " << std::endl;
        s.port_e300_ref().cal_connect();
    }

    // C1Qcal
    auto cmd_input = input_store.get_input_cmd<action, kind>(ta, oi);

    auto const sce = sensor_control_cal(q, s);
    cmd_input.sensor_control_enable = sce;

    // empty (default constructed) cmd for receiving
    using Co = typename Co<action, kind>::type;
    Co cmd_output{};

    md.send_recv(q.port_config, cmd_input, cmd_output, true);

    auto constexpr ui_id = UserInstruction::hash(action, kind);
    auto const task_id = ta.hash() + ui_id;

    // Co needs to be move_constructible
    output_store.cmd_map[task_id] =
        std::make_unique<Co>( std::move(cmd_output) );
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::show, Kind::target>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::target>(ta);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::show, Kind::plan>(TA const & ta, OI const & oi) {

    std::cout << std::endl << "show plan";
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::show, Kind::wait>(TA const & ta, OI const & oi) {

    // using sleep_until to discard the time streaming to cout
    auto const start_time = std::chrono::system_clock::now();

    std::chrono::seconds wait_seconds;

    if (oi.option.find('&') != std::string::npos) {
        wait_seconds = Utility::match_duration(oi.option);
    } else {
        wait_seconds = std::chrono::minutes(1);
    }

    std::cout << std::endl << wait_seconds << std::flush;

    int const cols = Utility::get_terminal_cols() - 4;
    auto const limit = static_cast<float>( wait_seconds.count() );

    std::cout << std::endl << "[";

    int pos = 0;
    int new_pos;

    for (std::chrono::seconds d(1); d <= wait_seconds; d++) {

        // std::this_thread::sleep_until(now + d);
        if ( Utility::cin_cancel(start_time + d) ) break;

        float const progress = d.count() / limit;
        new_pos = static_cast<int>(progress * cols);

        if (new_pos > pos) {
            std::cout << std::string(new_pos - pos, '=') << std::flush;
            pos = new_pos;
        }
    }

    std::cout << "]";
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::edit, Kind::plan>(TA const & ta, OI const & oi) {

    std::cout << std::endl << "edit plan";
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::set, Kind::ctrl>(TA const & ta, OI const & oi) {

    std::cout << std::endl << " !!!! sending reboot signal to digitizer !!!!";
    q_send_recv<Action::set, Kind::ctrl>(ta, oi);
    std::cout << std::endl << " !!!! reboot signal sent !!!!";
}

// -------------------------------------------------------------------------- //
/*
void Comm::last_reboot(Digitizer & q) {

    //TODO: change to an actual date
    C1Ping2 cmd_ping_2;
    cmd_ping_2.ping_type(2); // mini status request, empty

    C1Ping3 cmd_ping_3;
    md.send_recv(q.port_config, cmd_ping_2, cmd_ping_3);

    //cmd_ping_3.time_of_last_reboot();
}
*/

// *** REGISTRATION *** //
// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::set, Kind::reg>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);

    try {
        // DP Request server registration
        C1Rqsrv cmd_rqsrv;
        cmd_rqsrv.serial_number(q.config.serial_number);

        // Q Response: Server Challenge
        C1Srvch cmd_srvch;

        // dance
        md.send_recv(q.port_config, cmd_rqsrv, cmd_srvch, false);

        // Store this computer ip_address_number, for future get reg checking
        ip_address_number = cmd_srvch.server_ip_address();

        // DP Response to Q challenge
        C1Srvrsp cmd_srvrsp;

        // Set all values for DP Response
        cmd_srvrsp.serial_number(q.config.serial_number);
        cmd_srvrsp.challenge_value( cmd_srvch.challenge_value() );
        cmd_srvrsp.server_ip_address( cmd_srvch.server_ip_address() );
        cmd_srvrsp.server_udp_port( cmd_srvch.server_udp_port() );
        cmd_srvrsp.registration_number( cmd_srvch.registration_number() );

        // generate random value
        // this should happen only once during the program

        std::random_device rd;
        std::mt19937 gen( rd() );
        std::uniform_int_distribution<> dis(0, 9999999);
        cmd_srvrsp.server_random_value( dis(gen) );

        // md5
        std::stringstream md5_ss;

        md5_ss << std::hex      << std::setfill('0');
        md5_ss << std::setw(16) << cmd_srvrsp.challenge_value();
        md5_ss << std::setw(8)  << cmd_srvrsp.server_ip_address();
        md5_ss << std::setw(4)  << cmd_srvrsp.server_udp_port();
        md5_ss << std::setw(4)  << cmd_srvrsp.registration_number();
        md5_ss << std::setw(16) << q.port_config.auth_code;
        md5_ss << std::setw(16) << cmd_srvrsp.serial_number();
        md5_ss << std::setw(16) << cmd_srvrsp.server_random_value();

        std::array<uint8_t, 16> md5_digest = Md5::hash( md5_ss.str() );

        cmd_srvrsp.md5_result(md5_digest);

        // Q ack/cerr of DP Response
        C1Cack cmd_cack;

        md.send_recv(q.port_config, cmd_srvrsp, cmd_cack, false);

        std::cout << std::endl << "\n** Digitizer REGISTERED! ** " << std::endl;

        // setup status
        q.port_config.registered = true;

        // create task_id
        auto constexpr ui_id = UserInstruction::hash(Action::set, Kind::reg);
        auto const task_id = ta.hash() + ui_id;

        // move to cmd store
        output_store.cmd_map[task_id] =
            std::make_unique<C1Cack>( std::move(cmd_cack) );

    } catch (Exception const & e) {

        std::cerr << std::endl << "caught @Comm::run<set, reg>";
        q.port_config.registered = false;

        // error will trigger if a cerr is sent by the digitizer
        std::cerr << e.what();
        throw WarningException("Comm",
                               "registration",
                               "Registration failed");
    }
}

// *** DE - REGISTRATION *** //
// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::set, Kind::dereg>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);

    C1Dsrv cmd_dsrv;
    cmd_dsrv.serial_number(q.config.serial_number);

    // Q ack/cerr of DP Response
    C1Cack cmd_cack;

    md.send_recv(q.port_config, cmd_dsrv, cmd_cack, false);

    std::cout << "\n\n ** deregistered from digitizer ** \n";

    // setup status
    q.port_config.registered = false;

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::set, Kind::dereg);
    auto const task_id = ta.hash() + ui_id;

    // move to cmd store
    output_store.cmd_map[task_id] =
        std::make_unique<C1Cack>( std::move(cmd_cack) );
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::auto_, Kind::cal>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto & s = sn.s_ref(ta);

    // log this
    // ---------------------------------------------------------------------- //
    /*
    TODO: use a logging library, output should be file + console
    std::ofstream out;
    out.open("out.txt", std::ofstream::out | std::ofstream::trunc);
    //save old buffer
    std::streambuf * cout_buffer = std::cout.rdbuf();
    //redirect std::cout to out.txt
    std::cout.rdbuf( out.rdbuf() );
    */

    // use msg task mechanism for the plan
    // ---------------------------------------------------------------------- //
    // with start cal as individual action for when printing individual msg_task
    // since auto cal is a series of start cal
    auto const start_cal_ui = UserInstruction(Action::start,
                                              Kind::cal,
                                              oi.option);

    auto msg_tasks =
        cmd_file_reader_.construct_msg_tasks<Action::start, Kind::cal>(
            start_cal_ui,
            ta);

    Comm::run<Action::set, Kind::reg>(ta);
    auto const sce = sensor_control_cal(q, s);

    // the cal times depend on the digitizer time (seconds since epoch)
    // ---------------------------------------------------------------------- //
    for (auto & msg_task : msg_tasks) {
        auto * cal = dynamic_cast<C1Qcal *>( msg_task.cmd_send.get() );
        if (cal == nullptr) throw FatalException("Comm",
                                                 "autocal",
                                                 "cal nullptr");

        // settling_time is a duration, named following manual
        cal->starting_time( msg_task.exec_time() + cal->settling_time() );
        cal->sensor_control_enable = sce;
    }

    // stream full sequence
    // ---------------------------------------------------------------------- //
    if (msg_tasks.size() > 1) {
        std::cout << std::endl << "auto cal sequence for " << ta << ":\n";
        for (auto const & msg_task : msg_tasks) msg_task.stream<C1Qcal>(std::cout);
    }

    // e300 keep alive setup
    // ---------------------------------------------------------------------- //
    if (s.config.has_e300) {

        // plan cal might have been called before digitizer registration
        // where the e300 registration is done. This makes sure the e300
        // gets registered before proceeding. Also this is not caught here,
        // so if e300 registration fails, calibration fails, as it should.

        s.port_e300_ref().reg();

        // connect external calibration signal from e300

        s.port_e300_ref().cal_connect();

        // if the calibration plan takes more than an hour, the e300
        // needs to be kept awake or it will go back to "safe" mode
        // technically this only needs to happen if a single calibration
        // takes more than an hour.
        bool need_to_be_kept_alive = false;

        for (auto const & msg_task : msg_tasks) {
            if ( msg_task.run_duration() >= std::chrono::hours(1) ) {
                need_to_be_kept_alive = true;
                break;
            }
        }

        // for now the e300 gets kept alive their entire duration
        // TODO: use a vector of futures on keep_alive, to have different
        // keep alive periods if needed
        // right now it could do one long calibration like that
        // keeps alive on a separate thread
        if (need_to_be_kept_alive) {

            auto total_plan_run_duration = std::chrono::seconds(0);

            for (auto const & msg_task : msg_tasks) {
                total_plan_run_duration += msg_task.run_duration();
            }

            // give some time for registration and task creation
            auto constexpr keep_alive_delay = std::chrono::seconds(10);

            // sets cancel_keep_alive to false
            // but it can be set to true during the keep_alive_delay
            s.port_e300_ref().keep_alive(total_plan_run_duration,
                                         keep_alive_delay);
        }
    }

    // make sure cals are coordinated (not send a cal when there is one)
    // ---------------------------------------------------------------------- //
    auto cal_is_running = [&]() {

        // request status
        C1Rqstat cmd_rqstat;
        cmd_rqstat.request_bitmap.global_status(true);
        // status
        C1Stat cmd_stat;

        // needs to be registered
        md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);

        // get global status
        CxGlobalStatus * gs =
            dynamic_cast<CxGlobalStatus *>( cmd_stat.inner_commands[0].get() );

        if (gs == nullptr) throw FatalException("Comm",
                                                "autocal",
                                                "global stat nullptr");

        auto const & cs = gs -> calibrator_status;

        return ( cs.calibrator_should_be_generating_a_signal_but_isnt() or
                 cs.calibration_enable_is_on_this_second() or
                 cs.calibration_signal_is_on_this_second() );
    };

    // main loop of auto calibration
    // ---------------------------------------------------------------------- //
    try {

        // can't use delay since delay is meant for independently sent cals
        for (auto & msg_task : msg_tasks) {

            std::cout << std::endl << " ### now: "
                      << Time::sys_time_of_day() << " ###\n";

            Comm::run<Action::set, Kind::reg>(ta);

            // check if a calibration is going on
            auto cal_is_running_tries = 0;
            while ( cal_is_running() ) {

                // add some more wiggle time, digitizer still running cals
                auto constexpr wiggle_duration = std::chrono::seconds(5);
                std::this_thread::sleep_for(wiggle_duration);
                cal_is_running_tries++;
                std::cout << std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                                       << " another cal is running"
                                       << std::flush;

                if (cal_is_running_tries == 5) {
                    throw FatalException("Comm",
                                         "run<auto, cal>",
                                         "Calibrations are not coordinated");
                }
            }

            // ok ready to go
            msg_task.exec_time( std::chrono::system_clock::now() );
            msg_task.end_time( msg_task.exec_time() + msg_task.run_duration() );

            std::cout << "\nnext cal:\n";
            msg_task.stream<C1Qcal>(std::cout);

            md.send_recv( q.port_config,
                          *(msg_task.cmd_send.get()),
                          *(msg_task.cmd_recv.get()) );

            // no throw so far, received c1_cack
            msg_task.done = true;

            Comm::run<Action::set, Kind::dereg>(ta);

            if (msg_tasks.size() > 1) {
                // sleep on this thread, each msg task has the run_duration
                // already calculated.
                auto const sleep_duration = msg_task.run_duration();

                CmdFieldTime<> sleep_until_time;
                sleep_until_time(std::chrono::system_clock::now() + sleep_duration);

                std::cout << std::endl << " ### now: "
                          << Time::sys_time_of_day() << " ###\n";

                std::cout << std::endl << "sleep for: " << sleep_duration
                          << " until: " << sleep_until_time << std::endl;

                std::this_thread::sleep_for(sleep_duration);
            }
        }

        std::cout << std::endl << " ### now: "
                  << Time::sys_time_of_day() << " ###\n";

        std::cout << std::endl << "\nautocal success\n";

        // print out what was done
        for (auto & msg_task : msg_tasks) {
            std::cout << std::endl;
            msg_task.stream<C1Qcal>(std::cout);
        }

        // all done successfully, get the future from keep alive
        if (s.config.has_e300) s.port_e300_ref().wait_keep_alive();

    } catch (Exception const & e) {

        //std::cout.rdbuf(cout_buffer);

        // cancel keep alive thread and rethrow exception
        std::cerr << std::endl << "caught @Comm::run<auto, cal>";
        std::cerr << std::endl << "cancelling auto cal"
                  << "\n deregistering: \n";
        Comm::run<Action::set, Kind::dereg>(ta);

        if (s.config.has_e300) {
            std::cerr << "\n cancelling keep alive for e300: \n";
            // ok to set even if keep_alive(...)  was not called here
            s.port_e300_ref().cancel_keep_alive();
        }

        std::cerr << std::endl << "rethrow";
        throw e;
    }

    //std::cout.rdbuf(cout_buffer);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::auto_, Kind::stat>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_const_ref(ta);

    if ( not q_is_reg(q) ) Comm::run<Action::set, Kind::reg>(ta);

    // ---------------------------------------------------------------------- //
    if (s.config.has_e300) { std::cout << "\nTODO E300 auto stat"; return;}

    // ---------------------------------------------------------------------- //
    using Point = std::array<int16_t, 3>;

    // ---------------------------------------------------------------------- //
    auto constexpr number_of_axis = 3u;
    auto constexpr period = std::chrono::milliseconds(500);
    auto constexpr pps = 2u; // points per second (changes with period)
    auto constexpr ppl = 2u;  // points per line, changes plot looks

    StreamPlotter<int16_t, number_of_axis, pps, int8_t> sp(ppl);

    // changes the plot looks, adding a > when value at 127
    sp.min_limit = -126; sp.max_limit = 126;

    std::chrono::seconds loop_limit = std::chrono::minutes(2);

    for (std::chrono::milliseconds i(0); i < loop_limit; i += period) {

        Point const bp = boom_positions<Point>(q, s);

        sp.add(bp);
        sp.plot_lines();
        std::cout << std::flush;
        if ( Utility::cin_cancel(period) ) break;
    }
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::set, Kind::output>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_const_ref(ta);

    auto const ui = UserInstruction(Action::set,
                                    Kind::output,
                                    oi.option);

    // returns a vector for general case, only one needed
    auto cmds_ssc = cmd_file_reader_.construct_cmds<C1Ssc>(ui, ta);
    auto & cmd_ssc = cmds_ssc[0];

    // the intention is to be  able to set outputs for a specific sensor,
    // the outputs of the other sensor should be the same as the current one

    // get current sensor_control_map, C1Rqsc , C1Sc
    C1Rqsc cmd_rqsc;
    C1Sc cmd_sc;
    md.send_recv(q.port_config, cmd_rqsc, cmd_sc, false);

    if (s.config.input == Sensor::Input::a) {
        // sensor b outputs stay the same
        cmd_ssc.sensor_output_1b.line( cmd_sc.sensor_output_1b.line() );
        cmd_ssc.sensor_output_2b.line( cmd_sc.sensor_output_2b.line() );
        cmd_ssc.sensor_output_3b.line( cmd_sc.sensor_output_3b.line() );
        cmd_ssc.sensor_output_4b.line( cmd_sc.sensor_output_4b.line() );
        cmd_ssc.sensor_output_1b_active_high = cmd_sc.sensor_output_1b_active_high;
        cmd_ssc.sensor_output_2b_active_high = cmd_sc.sensor_output_2b_active_high;
        cmd_ssc.sensor_output_3b_active_high = cmd_sc.sensor_output_3b_active_high;
        cmd_ssc.sensor_output_4b_active_high = cmd_sc.sensor_output_4b_active_high;
    } else {
        // sensor a outputs stay the same
        cmd_ssc.sensor_output_1a.line( cmd_sc.sensor_output_1a.line() );
        cmd_ssc.sensor_output_2a.line( cmd_sc.sensor_output_2a.line() );
        cmd_ssc.sensor_output_3a.line( cmd_sc.sensor_output_3a.line() );
        cmd_ssc.sensor_output_4a.line( cmd_sc.sensor_output_4a.line() );
        cmd_ssc.sensor_output_1a_active_high = cmd_sc.sensor_output_1a_active_high;
        cmd_ssc.sensor_output_2a_active_high = cmd_sc.sensor_output_2a_active_high;
        cmd_ssc.sensor_output_3a_active_high = cmd_sc.sensor_output_3a_active_high;
        cmd_ssc.sensor_output_4a_active_high = cmd_sc.sensor_output_4a_active_high;
    }

    C1Cack cmd_cack;
    // only the first element on the vector is needed
    md.send_recv(q.port_config, cmd_ssc, cmd_cack);

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::set, Kind::output);
    auto const task_id = ta.hash() + ui_id;

    output_store.cmd_map[task_id] =
        std::make_unique<C1Cack>( std::move(cmd_cack) );
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::set, Kind::center>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_const_ref(ta);

    // --------- E300 registration ----------- //
    if (s.config.has_e300) throw InfoException("Comm",
                                               "run<set, center>",
                                               "TODO: set center for e300");
    using Minutes = std::chrono::minutes;

    // mass centering set
    auto constexpr maximum_tries = 5u;
    auto constexpr normal_interval = Minutes(2);
    auto constexpr squelch_interval = Minutes(3);

    // how does this work with invalid sensors (boom at 20)?
    // legitimate sensors can also take the value 20
    auto constexpr tolerance = 0;

    // the pulse needs to be in CentiSeconds.
    // (q330 manual "Duration 10 ms intervals")
    // using centi directly assures there is no truncation, using milliseconds
    // will be interpreted as possible truncation and will not compile unless
    // using floor to the expected type
    auto constexpr pulse_duration = std::chrono::seconds(2);

    C2Samass cmd_samass;

    auto const sce = sensor_control_center(q, s);

    if (s.config.input == Sensor::Input::a) {
        cmd_samass.pulse_duration_1(pulse_duration);
        cmd_samass.tolerance_1a(tolerance);
        cmd_samass.tolerance_1b(tolerance);
        cmd_samass.tolerance_1c(tolerance);
        cmd_samass.maximum_tries_1(maximum_tries);
        cmd_samass.normal_interval_1(normal_interval);
        cmd_samass.squelch_interval_1(squelch_interval);
        cmd_samass.sensor_control_enable_1 = sce;
    } else {
        cmd_samass.pulse_duration_2(pulse_duration);
        cmd_samass.tolerance_2a(tolerance);
        cmd_samass.tolerance_2b(tolerance);
        cmd_samass.tolerance_2c(tolerance);
        cmd_samass.maximum_tries_2(maximum_tries);
        cmd_samass.normal_interval_2(normal_interval);
        cmd_samass.squelch_interval_2(squelch_interval);
        cmd_samass.sensor_control_enable_2 = sce;
    }

    C1Cack cmd_cack;
    md.send_recv(q.port_config, cmd_samass, cmd_cack);

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::set, Kind::center);
    auto const task_id = ta.hash() + ui_id;

    output_store.cmd_map[task_id] =
        std::make_unique<C1Cack>( std::move(cmd_cack) );
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::start, Kind::center>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_const_ref(ta);

    // ---------------------------------------------------------------------- //
    auto centering_is_running = [&]() {

        // request status
        C1Rqstat cmd_rqstat;
        cmd_rqstat.request_bitmap.global_status(true);
        // status
        C1Stat cmd_stat;

        // needs to be registered
        md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);

        // get global status
        CxGlobalStatus * gs =
            dynamic_cast<CxGlobalStatus *>( cmd_stat.inner_commands[0].get() );

        if (gs == nullptr) throw FatalException("Comm",
                                                "autocal",
                                                "global stat nullptr");

        auto const & scm = gs -> sensor_control_enable();

        return ( static_cast<bool>( scm.to_ulong() ) );
    };

    // make sure another centering is not running
    // ---------------------------------------------------------------------- //
    if ( centering_is_running() ) {
        throw FatalException("Comm",
                             "run<auto, cal>",
                             "Calibrations are not coordinated");
    }

    // ---------------------------------------------------------------------- //
    // pulse setup for mass centering
    C1Pulse cmd_pulse;

    // the pulse needs to be in CentiSeconds.
    // (q330 manual "Duration 10 ms intervals")
    // using centi directly assures there is no truncation, using milliseconds
    // will be interpreted as possible truncation and will not compile unless
    // using floor to the expected type
    std::chrono::duration<uint16_t, std::centi> pulse_duration;

    if (oi.option.find('&') != std::string::npos) {
        pulse_duration = Utility::match_duration(oi.option);
    } else {
        pulse_duration = std::chrono::seconds(2);
    }

    auto constexpr consensus_max_pulse_duration = std::chrono::seconds(10);
    if (pulse_duration > consensus_max_pulse_duration) {
        throw WarningException("Comm",
                               "run <start, center>",
                               "pulse duration > 10 seconds");
    }

    cmd_pulse.pulse_duration(pulse_duration);

    auto const sce = sensor_control_center(q, s);
    cmd_pulse.sensor_control_enable = sce;

    C1Cack cmd_cack;
    md.send_recv(q.port_config, cmd_pulse, cmd_cack);

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::start, Kind::center);
    auto const task_id = ta.hash() + ui_id;

    output_store.cmd_map[task_id] =
        std::make_unique<C1Cack>( std::move(cmd_cack) );
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::auto_, Kind::center>(TA const & ta, OI const & oi) {


    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_const_ref(ta);

    if ( not q_is_reg(q) ) Comm::run<Action::set, Kind::reg>(ta);

    // ---------------------------------------------------------------------- //
    if (s.config.has_e300) { std::cout << "\nTODO E300 auto center"; return;}

    // ---------------------------------------------------------------------- //
    using Point = std::array<int16_t, 3>;

    auto constexpr tolerance = 10;
    // ---------------------------------------------------------------------- //
    auto centered = [](Point const & bp, auto const & tolerance) {
        for (auto const & bp_axis : bp) {
            if (std::abs(bp_axis) > tolerance) return false;
        }
        return true;
    };

    // ---------------------------------------------------------------------- //
    auto stream_bp = [](auto const & bp) {
        std::cout << "[";
        for (auto const & bp_axis : bp) {
            std::cout << std::setw(5) << bp_axis << " ";
        }
        std::cout << "]";
    };

    std::vector<Point> bps;
    auto constexpr max_attempts = 5u;
    bool success = false;

    OptionInput const oi_show_wait("&2m");
    OptionInput const oi_auto_stat("boom");
    // should match show wait (2m) and auto stat (2m)
    auto constexpr sleep_duration = std::chrono::minutes(4);
    auto attempt = 0;

    // ---------------------------------------------------------------------- //
    for (attempt = 0; attempt < max_attempts; attempt++) {

        Point const bp = boom_positions<Point>(q, s);
        bps.push_back(bp);
        success = centered(bp, tolerance);
        if (success) break;

        auto const start_time = std::chrono::system_clock::now();

        std::cout << "\ntolerance      : " << tolerance;
        std::cout << "\nmass positions : "; stream_bp(bp);

        // lets try to center this
        run<Action::start, Kind::center>(ta, oi);
        std::cout << std::endl << "mass center updates:";
        run<Action::auto_, Kind::stat>(ta, oi_auto_stat);
        std::cout << std::endl << "wait before checking centers again";
        run<Action::show,  Kind::wait>(ta, oi_show_wait);
        // force the wait even without stream output, if both previous
        // instructions are not interrupted, this should sleep almost nothing
        std::this_thread::sleep_until(start_time + sleep_duration);
        std::cout << std::flush;
    }

    // ---------------------------------------------------------------------- //
    auto const & st = sn.st[ta.st_child.index];

     std::cout << "\n---------------------------------------"
               << "\n      *** auto center summary ***      "
               << "\n---------------------------------------";

    std::cout << "\nstation   : " << st.config.station_name
              << "\ndigitizer : " << q
              << "\nsensor    : " << s;


    std::cout << "\n\nresult    : " << (success ? "centered" : "!! failure")
              <<   "\nattempts  : " << attempt
              <<   "\ntolerance : " << tolerance;

    std::cout << "\n\nmass positions :";

    for (auto const bp : bps) {
        std::cout << "\n";
        stream_bp(bp);
    }

    std::cout << std::endl << "\n### "
              << Time::sys_year_month_day() << " "
              << "julian(" << Time::julian_day() << ") "
              << Time::sys_time_of_day() << " ###";
}

// *** QUICK VIEW *** //
// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::auto_, Kind::qview>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);

    if ( not q_is_reg(q) ) Comm::run<Action::set, Kind::reg>(ta);

    C2Rqqv cmd_rqqv;
    C2Qv cmd_qv;

    auto const & s = sn.s_const_ref(ta);

    if (s.config.input == Sensor::Input::a) {
        cmd_rqqv.channel_map.channel_1(true);
        cmd_rqqv.channel_map.channel_2(true);
        cmd_rqqv.channel_map.channel_3(true);
    } else {
        cmd_rqqv.channel_map.channel_4(true);
        cmd_rqqv.channel_map.channel_5(true);
        cmd_rqqv.channel_map.channel_6(true);

    }

    using Point = std::array<int32_t, 3>;

    // initialized with current, updated with the received ones
    int qv_seq_number = q_current_seq_number(q);

    // ---------------------------------------------------------------------- //
    auto qview_values = [&]() {

        // "39 byte differences from starting value (40 bytes actually used)"
        // the last point in the bit difference vector is always zero.
        // it provides 40 points but only 39 are usable.
        // the first point (absolute value) of the 40 is actually given by
        // starting value, the other 39 are calculated using the differences
        auto constexpr N = 40u;
        std::vector<Point> qview_values(N);

        // you can get data from before seq_number_
        // then you get several qv values
        // now we would have to deal either with overlap or a small gap
        cmd_rqqv.lowest_sequence_number(qv_seq_number);

        md.send_recv(q.port_config, cmd_rqqv, cmd_qv, false);

        // update values for next qv, assuming one
        qv_seq_number = cmd_qv.starting_sequence_number() +
                        cmd_qv.seconds_count().count();

        // get the values
        for (int axis = 0; axis < cmd_qv.inner_commands.size(); axis++) {

            auto & cmd_cx_qv = cmd_qv.inner_commands[axis];
            CxQv * qv = dynamic_cast<CxQv *>( cmd_cx_qv.get() );

            if (qv == nullptr) throw std::logic_error{"@Comm::qview"};

            // "Each [byte] difference must be multiplied by (1 << shift count)"
            // CmdField<uint16_t, 2> shift_count;
            auto const shift_count = qv -> shift_count();
            auto const multiplier = 1 << shift_count;

            auto const starting_value = qv -> starting_value();

            qview_values[0][axis] = starting_value;

            // takes data differences, which are signed bytes
            // CmdField<std::array<int8_t, 40>, 40> byte_difference;
            int8_t data_point;

            // first point already taken
            for (int i = 1; i < N; i++) {
                data_point = qv -> byte_difference()[i];
                auto const starting_diff = data_point * multiplier;
                qview_values[i][axis] = starting_value + starting_diff;
            }
        }

        return qview_values;
    };

    // number is 26 bit signed integer, T and Tc both int32_t
    // ---------------------------------------------------------------------- //
    auto constexpr number_of_axis = 3;
    auto constexpr ppl = 20;  // points per line, changes plot looks
    auto constexpr pps = 40; // points per second (property of qview)
    StreamPlotter<int32_t, number_of_axis, pps> sp(ppl);

    // just changes the plot looks
    sp.min_limit = -33'000'000; sp.max_limit = 33'000'000;

    // 16 minutes, less than 999 seconds
    auto constexpr max_loop_limit = std::chrono::minutes(15);

    std::chrono::seconds loop_limit;
    if (oi.option.find('&') == std::string::npos) loop_limit = max_loop_limit;
    else loop_limit = Utility::match_duration(oi.option);

    // do not change the period
    auto constexpr period = std::chrono::seconds(1);

    for (std::chrono::seconds i(0); i < loop_limit; i += period) {

        auto const qvv = qview_values();

        sp.add(qvv);
        sp.plot_lines();
        if ( Utility::cin_cancel(period) ) break;
    }

    sp.set_ppl(4);
    std::cout << std::endl;
    sp.plot_all();

}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::quit, Kind::mzn>(TA const & ta, OI const & oi) {
    // TODO clean up mtm
    // md.join_timed_threads();
}

//TODO: compound, utility function, move separetely?
// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::get, Kind::uptime>(TA const & ta, OI const & oi) {

    auto & st = sn.st_ref(ta);

    std::ofstream uptime_report_fs;

    uptime_report_fs.open("uptime_report.txt",
          std::ofstream::out | std::ofstream::trunc);

    uptime_report_fs << "\nstation: " << st.config.station_name << std::endl;

    for (auto & q : st.q) {

        uptime_report_fs << "\n    digitizer: ";
        uptime_report_fs << "    " << std::hex
                         << q.config.serial_number
                         << std::dec;

        uptime_report_fs << "\n    time of last reboot: ";
        // uptime_report_fs <<  last_reboot(q)  ;
    }

    uptime_report_fs << "\n    data processor: uptime:\n ";
    uptime_report_fs << "    " << st.dp[0].uptime();

    uptime_report_fs.close();
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::edit, Kind::stat>(TA const & ta, OI const & oi) {
    //TODO: use type traits
    //TODO: show users shortcut and live options
    // cmd_show_options<C1Ping4, Action::edit, Kind::stat>();
}

// -------------------------------------------------------------------------- //
// for stream_output
// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::show, Kind::config>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::config>(ta);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::show, Kind::status>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::status>(ta);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::show, Kind::help>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::help>(ta);
}

// -------------------------------------------------------------------------- //
template<>
inline
void Comm::run<Action::start, Kind::link>(TA const & ta, OI const & oi) {

    auto & s = sn.s_ref(ta);

    // --------- E300 registration ----------- //
    if (not s.config.has_e300) {
        throw WarningException("Comm",
                               "run<start, link>",
                               "this sensor does not have E300 setup");
    }

    auto & e300 = s.port_e300_ref();

    e300.connect();

    std::cout << std::endl << "input commands for E300, type quit to quit";
    std::string input{};

    while (true) {
        std::cout << "\n_______________________________________";
        std::cout << std::endl << "E300> ";
        std::getline(std::cin, input);
        if (input == "quit") break;
        auto const response = e300.send_recv(input);
        std::cout << response;
    }
}


} // end namespace
#endif // _MZN_COMM_H_
