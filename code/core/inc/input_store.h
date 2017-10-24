// Manzano software
#ifndef _MZN_INPUT_STORE_H_
#define _MZN_INPUT_STORE_H_

#include <map>
#include <memory>
#include "commands.h"
#include "user_instruction.h"
#include "seismic_network.h"
#include "ui_to_cmd_type.h"

namespace mzn {

//! Modifies input commands if needed
/*!
    The modification can be the needed default due that cmd_fields
    and the commands that hold them use a default initialization according
    to the type but require to provide at least one option different than
    the default initialization e.g. for "get ping":
        ping_type = 4 for C1_Ping

    or can be sensible default e.g. for "get stat":
        request_bitmap.set_global_status(true) for C1_Stat

    or can use a shortcut option e.g. for "get stat:boom":
        request_bitmap.set_boom_positions(true) for C1_Stat

    or can use live option e.g. for "get stat/my_live_option":
        request_bitmap.set_global_status(true)
        request_bitmap.set_boom_positions(true)

    more information on the terms:
    shortcut option: uses a custom input command known at compile time
    that can reliably called using the option e.g. stat:power
    by all users at any time and cannot be modified at run time

    live option: uses a custom input command that was created at run time
    like a special calibration configuration command
    the input command can also be modified at run time
    (possible future feature is to save/load to file these live custom commands)

    FUTURE: with C++17, the static member variable can be made in a better way
    @author rfigueroa@usgs.gov
 */

// -------------------------------------------------------------------------- //
class InputStore {

private:
    SeismicNetwork const & sn_;

public:
    explicit
    InputStore(SeismicNetwork const & sn);
    ~InputStore() = default;

    //! used to show the compile time shortcut options
    //! specializations provide options when available
    // --------------------------------------------------------------------- //
    template <Action action, Kind kind>
    inline
    std::vector<std::string> shortcut_options() {
        return std::vector<std::string>{};
    }

    //! used to show the currently available options
    // --------------------------------------------------------------------- //
    template <Action action, Kind kind>
    inline
    std::vector<std::string> live_options() {

        /*
        using Ci = typename Ci<action, kind>::type;
        std::vector<std::string> options{};

        // the ui hash is the key for live_cmd_map
        auto const map_ui_hash = UserInstruction(action, kind)();

        // search for live options in storage
        auto const live_cmd_map_iter = live_cmd_map.find(map_ui_hash);

        // not found in storage, currently there are no live options
        // for this action/kind combination, return empty vector
        if ( live_cmd_map_iter == live_cmd_map.end() ) return options;

        // there are live options
        // live_cmd_map has another map option to cmd ptr accesible using:
        // where cmd_map_iter -> second

        //TODO: reserve?
        for (auto const & option_to_map_iter : live_cmd_map_iter -> second) {
            // we are only interested in the key (option)
            options.push_back(option_to_map_iter.first);
        }

        return options;
        */
    }

    // --------------------------------------------------------------------- //
    template <Action action, Kind kind,
              class Ci = typename Ci<action, kind>::type >
    inline
    Ci get_input_cmd(TargetAddress const & ta, OptionInput const & oi) {

        if (oi.live) return get_live_cmd<action, kind>(ta, oi.option);
        return get_default_cmd<action, kind>(ta, oi.option);
    }

    // specialized functions for commands that have shortcuts and/or
    // need changes to the default Ci constructor
    // --------------------------------------------------------------------- //
    template <Action action, Kind kind,
              class Ci = typename Ci<action, kind>::type >
    inline
    Ci get_default_cmd(TargetAddress const & ta, std::string const & option) {
        return Ci{};
    }

    //! An option when the specific command type is not needed
    //! and action/kind is not known at compile time
    //! only the output of the command is needed
    // --------------------------------------------------------------------- //
    template <Action action, Kind kind>
    inline
    std::string get_string_input(TargetAddress const & ta,
                                 OptionInput const & oi) {

        if (oi.live) return std::string{};

        auto cmd_input = get_default_cmd<action, kind>(oi.option, ta);
        std::stringstream ss;
        ss << cmd_input;
        return ss.str();
    }

    // --------------------------------------------------------------------- //
    template <Action action, Kind kind>
    void add_live_cmd(TargetAddress const & ta, std::string const & option) {
        //using Ci = typename Ci<action, kind>::type;
        //cmd_map[option] = std::make_unique<Ci>(Ci{});
    }

    // --------------------------------------------------------------------- //
    template <Action action, Kind kind,
              class Ci = typename Ci<action, kind>::type >
    inline
    Ci get_live_cmd(TargetAddress const & ta, std::string const & option) {

        /*
        // search for the option in storage
        auto const cmd_map_iter = cmd_map.find(option);

        // not found in storage
        if ( cmd_map_iter == cmd_map.end() ) throw_bad_option(option);

        // found in storage
        std::cout << *(cmd_map_iter -> second);

        C * cmd_ptr = dynamic_cast<C *>( (cmd_map_iter -> second).get() );

        std::cout << "\nworked";
        if (cmd_ptr == nullptr) throw_bad_option(option);

        return C{*cmd_ptr};
        */
        return Ci{};
    }


private:

    using OptionCmdMap = std::map < std::string, std::unique_ptr<Command> >;

    //! for run-time, on-the-fly custom commands
    static
    std::map<uint16_t, OptionCmdMap> mpid_options_map;

    // --------------------------------------------------------------------- //
    inline
    static
    void throw_bad_option(std::string const & option) {
        throw WarningException("InputStore",
                               "get_input_cmd",
                               "option \"" + option + "\" not found");
    }
};

// TODO: move to cpp?
// -------------------------------------------------------------------------- //
template<>
inline
std::string InputStore::get_string_input<Action::set, Kind::reg> (
        TargetAddress const & ta,
        OptionInput const & oi) {

    return std::string{"REGISTER ->"};
}

// -------------------------------------------------------------------------- //
template<>
inline
std::string InputStore::get_string_input<Action::set, Kind::dereg> (
        TargetAddress const & ta,
        OptionInput const & oi) {

    return std::string{"DE-REGISTER ->"};
}

// -------------------------------------------------------------------------- //
template<>
inline
typename Ci<Action::get, Kind::ping>::type
InputStore::
get_default_cmd<Action::get, Kind::ping>(TargetAddress const & ta,
                                         std::string const & option) {
    //  unless there an option is provided and is in the cmd_map
    //if (ui.option != "") return get_live_cmd<C1Ping4>(ui.option);

    using Ci = typename Ci<Action::get, Kind::ping>::type;
    Ci cmd;

    if (option == "") cmd.ping_type(4); else
    throw_bad_option(option);

    return cmd;
}

// -------------------------------------------------------------------------- //
template<>
inline
typename Ci<Action::get, Kind::stat>::type
InputStore::
get_default_cmd<Action::get, Kind::stat>(TargetAddress const & ta,
                                         std::string const & option) {

    //  unless there an option is provided and is in the cmd_map
    //if (ui.option != "") return get_live_cmd<C1Ping4>(ui.option);
    using Ci = typename Ci<Action::get, Kind::stat>::type;
    Ci cmd;

    if (option == "") cmd.request_bitmap.global_status(true); else
    if (option == "boom") cmd.request_bitmap.boom_positions(true); else
    if (option == "gps") cmd.request_bitmap.gps_status(true); else
    if (option == "gpssat") cmd.request_bitmap.gps_satellites(true); else
    if (option == "power") cmd.request_bitmap.power_supply_status(true); else
    if (option == "thread") cmd.request_bitmap.thread_status(true); else

    if (option == "dataport") {
        cmd.request_bitmap.data_port_1_status(true);
        cmd.request_bitmap.data_port_2_status(true);
        cmd.request_bitmap.data_port_3_status(true);
        cmd.request_bitmap.data_port_4_status(true);
    } else

    if (option == "pll") cmd.request_bitmap.pll_status(true); else
    throw_bad_option(option);

    return cmd;
}

// -------------------------------------------------------------------------- //
template<>
inline
typename Ci<Action::get, Kind::token>::type
InputStore::
get_default_cmd<Action::get, Kind::token>(TargetAddress const & ta,
                                          std::string const & option) {

    //  unless there an option is provided and is in the cmd_map
    //if (ui.option != "") return get_live_cmd<C1Ping4>(ui.option);
    using Ci = typename Ci<Action::get, Kind::token>::type;
    using MT = BmMemoryType::MemoryType;
    Ci cmd;
    cmd.byte_count(0);

    if (option == "1") cmd.memory_type.memory_type(MT::data_port_1); else
    if (option == "2") cmd.memory_type.memory_type(MT::data_port_2); else
    if (option == "3") cmd.memory_type.memory_type(MT::data_port_3); else
    if (option == "4") cmd.memory_type.memory_type(MT::data_port_4); else
    throw_bad_option(option);

    // no requirements to read these
    std::array<uint8_t, 16> pw {0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0};
    cmd.pw(pw);

    return cmd;
}

// -------------------------------------------------------------------------- //
template<>
inline
typename Ci<Action::set, Kind::token>::type
InputStore::
get_default_cmd<Action::set, Kind::token>(TargetAddress const & ta,
                                          std::string const & option) {
    //  unless there an option is provided and is in the cmd_map
    //if (ui.option != "") return get_live_cmd<C1Ping4>(ui.option);
    using Ci = typename Ci<Action::set, Kind::token>::type;
    using MT = BmMemoryType::MemoryType;
    Ci cmd;

    if (option == "1") cmd.memory_type.memory_type(MT::data_port_1); else
    if (option == "2") cmd.memory_type.memory_type(MT::data_port_2); else
    if (option == "3") cmd.memory_type.memory_type(MT::data_port_3); else
    if (option == "4") cmd.memory_type.memory_type(MT::data_port_4); else
    throw_bad_option(option);

    return cmd;
}
// -------------------------------------------------------------------------- //
template<>
inline
typename Ci<Action::start, Kind::cal>::type
InputStore::
get_default_cmd<Action::start, Kind::cal>(TargetAddress const & ta,
                                          std::string const & option) {

    using Ci = typename Ci<Action::start, Kind::cal>::type;
    Ci cmd;

    using Seconds = std::chrono::seconds;
    using Minutes = std::chrono::minutes;
    // durations in seconds
    if (option == "step") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::step);
        cmd.amplitude(-6);
        cmd.settling_time( Seconds(0) );
        cmd.cal_duration( Minutes(2) );
        cmd.trailer_time( Seconds(0) );

    } else if (option == "sine") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::sine);
        cmd.amplitude(-6);
        cmd.settling_time( Minutes(2) );
        cmd.cal_duration( Minutes(2) );
        cmd.trailer_time( Minutes(2) );
        cmd.frequency_divider(1); // 1hz for sine waveform

    } else if (option == "random") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::sine);
        cmd.amplitude(-6);
        cmd.settling_time( Minutes(0) );
        cmd.cal_duration( Minutes(2) );
        cmd.trailer_time( Minutes(1) );
        cmd.frequency_divider(1); // 1hz for sine waveform

    } else if (option == "longstep") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::step);
        cmd.amplitude(-6);
        cmd.settling_time( Minutes(15) );
        cmd.cal_duration( Minutes(15) );
        cmd.trailer_time( Minutes(15) );

    } else if (option == "longsine") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::sine);
        cmd.amplitude(-6);
        cmd.settling_time( Minutes(2) );
        cmd.cal_duration( Minutes(10) );
        cmd.trailer_time( Minutes(5) );
        cmd.frequency_divider(1); // 1hz for sine waveform

    } else if (option == "hfw6") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::white_noise);
        cmd.amplitude(-6);
        cmd.settling_time( Minutes(10) );
        cmd.cal_duration( Minutes(20) );
        cmd.trailer_time( Minutes(10) );
        cmd.frequency_divider(1); // 125hz for noise waveform

    } else if (option == "hfr6") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::random);
        cmd.amplitude(-6);
        cmd.settling_time( Minutes(10) );
        cmd.cal_duration( Minutes(20) );
        cmd.trailer_time( Minutes(10) );
        cmd.frequency_divider(1); // 125hz for noise waveform

    } else if (option == "hfw18") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::white_noise);
        cmd.amplitude(-18);
        cmd.settling_time( Minutes(10) );
        cmd.cal_duration( Minutes(20) );
        cmd.trailer_time( Minutes(10) );
        cmd.frequency_divider(1); // 125hz for noise waveform

    } else if (option == "hfr18") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::random);
        cmd.amplitude(-18);
        cmd.settling_time( Minutes(10) );
        cmd.cal_duration( Minutes(20) );
        cmd.trailer_time( Minutes(10) );
        cmd.frequency_divider(1); // 125hz for noise waveform

    } else throw_bad_option(option);

    // Need to setup basic command before choosing options
    auto const & s = sn_.s_const_ref(ta);

    // sensor calibration channel first
    if (s.config.input == Sensor::Input::a) {
        cmd.calibration_bitmap.input(BmCalibrationBitmap::Input::a);
        cmd.monitor_channel_bitmap.channel_4(true);
    } else {
        cmd.calibration_bitmap.input(BmCalibrationBitmap::Input::b);
        cmd.monitor_channel_bitmap.channel_1(true);
    }

    cmd.waveform.automatic_calibration(true);
    cmd.waveform.negative_step(false);

    std::array<char, 12> constexpr
        coupling_bytes {{'r','e','s','i','s','t','i','v','e'}};

    cmd.coupling_bytes(coupling_bytes);

    cmd.starting_time( std::chrono::system_clock::now() + cmd.settling_time() );

    //! Important: sensor control enable missing
    //! done in Comm::run, needs to check curreng digitizer configuration

    return cmd;
}

// -------------------------------------------------------------------------- //
template<>
inline
typename Ci<Action::set, Kind::ctrl>::type
InputStore::
get_default_cmd<Action::set, Kind::ctrl>(TargetAddress const & ta,
                                         std::string const & option) {

    using Ci = typename Ci<Action::set, Kind::ctrl>::type;
    Ci cmd;

    // no default command without explicit option
    if (option == "save")
        cmd.ctrl_flags.save_current_programming_to_eeprom(true); else

    if (option == "reboot") cmd.ctrl_flags.reboot(true); else
    if (option == "resync") cmd.ctrl_flags.re_sync(true); else
    if (option == "gps_on") cmd.ctrl_flags.turn_gps_on(true); else
    if (option == "gps_off") cmd.ctrl_flags.turn_gps_off(true); else
    if (option == "gps_cold_start") cmd.ctrl_flags.cold_start_gps(true); else
    throw_bad_option(option);

    return cmd;
}
} // end namespace

#endif // _MZN_INPUT_STORE_H_
