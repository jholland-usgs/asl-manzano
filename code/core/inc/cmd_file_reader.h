#ifndef _MZN_CMD_FILE_READER_H_
#define _MZN_CMD_FILE_READER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#include "commands.h"
#include "seismic_network.h"
#include "msg_task.h"
#include "ui_to_cmd_type.h"
#include "system_calls.h"
#include "json_sn.h"
#include "json_cals.h"

namespace mzn {

//! Creates C1Qcal cmds from cal config files and timed sends them
/*!
    @author rfigueroa@usgs.gov
 */

// -------------------------------------------------------------------------- //
class CmdFileReader {

    using Seconds = std::chrono::seconds;

    //! same lifetime as CmdFieldReader
    SeismicNetwork const & sn_;

public:
    explicit
    CmdFileReader(SeismicNetwork const & sn);

    ~CmdFileReader() = default;

    //! no copy construction or assignment
    CmdFileReader(CmdFileReader const & rhs) = delete;
    CmdFileReader & operator=(CmdFileReader const & rhs) = delete;

    //! calls construct_cmds and package them in tasks
    template <Action action, Kind kind,
    class Ci = typename Ci<action, kind>::type,
    class Co = typename Co<action, kind>::type >
    std::vector<MsgTask> construct_msg_tasks(UserInstruction const & ui,
                                             TargetAddress const & ta);

    //! read cals cal sequences config file and setup cal_tasks_;
    template <typename Ci>
    std::vector<Ci> construct_cmds(UserInstruction const & ui,
                                   TargetAddress const & ta);

private:
    //! checks json format and throws if error
    //! @throws if json format error
    void check_json(bool const check, std::string const & e_what);

    //! when using msg_task_manager, the delays allows for independent delayed
    //! msg sending, calibrations not currently using the msg_task_manager
    std::vector<Seconds>
    calculate_delays(std::vector<Seconds> const & run_durations);

    //! total duration, i.e.: for calibrations includes settling time
    template <typename Ci>
    std::vector<Seconds> calculate_run_durations(std::vector<Ci> const & cmds);
};

// -------------------------------------------------------------------------- //
template <Action action, Kind kind, class Ci, class Co>
inline
std::vector<MsgTask>
CmdFileReader::construct_msg_tasks(UserInstruction const & ui,
                                   TargetAddress const & ta) {

    auto const cmds = construct_cmds<Ci>(ui, ta);
    auto const run_durations = calculate_run_durations(cmds);
    auto const delays = calculate_delays(run_durations);

    Co const cmd_recv;

    // --- package into tasks ---
    std::vector<MsgTask> msg_tasks{};

    auto package_tasks = [&](auto const & i) {

        MsgTask msg_task(cmds[i],
                         cmd_recv,
                         delays[i],
                         run_durations[i],
                         ui,
                         ta);

        msg_tasks.push_back( std::move(msg_task) );
    };

    // process all cmds
    for (unsigned i = 0; i < cmds.size(); i++) package_tasks(i);

    // error
    return msg_tasks;
}

// -------------------------------------------------------------------------- //
template <>
inline
std::vector<C1Qcal>
CmdFileReader::construct_cmds(UserInstruction const & ui,
                              TargetAddress const & ta) {

    auto const & option = ui.option_input.option;
    bool const option_is_positive_number = Utility::is_positive_number(option);
    bool const option_is_cal_name =
        not (option.empty() or option_is_positive_number);

    // is the expected cal sequence on the cal sequence list?
    auto const & s = sn_.s_const_ref(ta);
    auto const sensor_cals_json = option_is_cal_name ?
                                  Utility::get_cals_json(option) :
                                  Utility::get_cals_json(s.config.cals);

    // --------------------------------------------------------------------- //
    std::vector<C1Qcal> cmds;

    // reserve the space in the cals vector
    cmds.reserve( sensor_cals_json.size() ) ;

    // ********** from config to real commands *********** //
    auto qcal_from_json = [&](auto const & i) {

        C1Qcal cal{};

        // monitor and channel are the same for all the calibrations
        // in one calibration object in the cal_sequences file
        // currently being taken from the *** Sensor *** object
        // not the autocal sequences file. So that the the autocal config
        // is independent of how the sensors are connected into the digitizer

        // ------------- CALIBRATION CHANNEL -------------------- //
        // ------------- MONITOR CHANNEL -------------------- //
        // monitor channel is being defaulted (for autocals)
        // to the middle channel of the other input
        if (s.config.input == Sensor::Input::a) {

            cal.calibration_bitmap.input(BmCalibrationBitmap::Input::a);
            // TODO: I need the current SCM
            // cal.sensor_control_enable.;
            cal.monitor_channel_bitmap.channel_4(true);

        } else if (s.config.input == Sensor::Input::b)  {

            cal.calibration_bitmap.input(BmCalibrationBitmap::Input::b);
            // TODO: I need the current SCM
            // cal.sensor_control_map.lines(SCML::sensor_b_calibration);
            cal.monitor_channel_bitmap.channel_1(true);
        }

        // ------------- WAVEFORM -------------------- //
        std::string const waveform_json = sensor_cals_json[i]["wave_form"];

        // in this case, no match = sine, since Waveform::sine = 0
        if (waveform_json == "sine") {
            cal.waveform.waveform(BmCalWaveform::Waveform::sine);
        } else if (waveform_json == "white_noise") {
            cal.waveform.waveform(BmCalWaveform::Waveform::white_noise);
        } else if (waveform_json == "red_noise") {
            cal.waveform.waveform(BmCalWaveform::Waveform::red_noise);
        } else if (waveform_json == "step") {
            cal.waveform.waveform(BmCalWaveform::Waveform::step);
        } else if (waveform_json == "random") {
            cal.waveform.waveform(BmCalWaveform::Waveform::random);
        } else {
            check_json(false, "waveform in cals is not recognized");
        }

        // default positive step and automatic calibration for autocal
        cal.waveform.negative_step(false);
        cal.waveform.automatic_calibration(true);

        // ------------- AMPLITUDE -------------------- //
        int const amplitude_json = sensor_cals_json[i]["amplitude"];

        cal.amplitude(amplitude_json);

        // ------------- DURATIONS -------------------- //
        auto duration_in_seconds = [](unsigned int h, unsigned int m) {
            return std::chrono::seconds(h*3600 + m*60);
        };

        cal.cal_duration(
            duration_in_seconds(
                sensor_cals_json[i]["cal_duration"]["h"],
                sensor_cals_json[i]["cal_duration"]["m"]
            )
        );

        cal.settling_time(
            duration_in_seconds(
                sensor_cals_json[i]["settling_time"]["h"],
                sensor_cals_json[i]["settling_time"]["m"]
            )
        );

        cal.trailer_time(
            duration_in_seconds(
                sensor_cals_json[i]["trailer_time"]["h"],
                sensor_cals_json[i]["trailer_time"]["m"]
            )
        );

        // ------------- FREQUENCY -------------------- //
        // TODO: check_json when waveform is sine and maybe others?

        if ( sensor_cals_json[i].find("frequency") != sensor_cals_json[i].end() ) {

            /* from the manual:
            The Frequency Divider is used to reduce the frequency
            of a sine or noise waveform.
            A value of 1 generates a 1Hz sine or update a noise waveform at 125Hz.
            A value of 20 generates a 0.05Hz sine or
            update a noise waveform at 6.25Hz.
            Max. value is 255.
            */

            int frequency_divider;
            double const frequency = sensor_cals_json[i]["frequency"];

            if ( waveform_json == "sine" ) {
                frequency_divider = static_cast<int> (1.0 / frequency);
            } else {
                frequency_divider = static_cast<int> (125.0 / frequency);
            }

            cal.frequency_divider(frequency_divider);
        }

        std::array<char, 12> constexpr coupling_bytes
            { {'r','e','s','i','s','t','i','v','e'} };

        cal.coupling_bytes(coupling_bytes);

        // Important: sensor control enable is being done in Comm::run

        // add to vector of calibrations
        cmds.push_back(cal);
    };

    // check if only a specific cmd is desired
    if (option_is_positive_number) {

        std::size_t token_index = 0;

        auto const restrict_to_cmd_index =
            Utility::match_positive_number(ui.option_input.option, token_index);

        if ( restrict_to_cmd_index >= sensor_cals_json.size() )  {
            throw WarningException("CmdFileReader",
                                   "construct_cmds<QCal>",
                                   "specified index is outside the bounds");
        }

        qcal_from_json(restrict_to_cmd_index);

    } else {
        // do all the relevant commands in the file
        for (auto i = 0; i < sensor_cals_json.size(); i++) qcal_from_json(i);
    }

    return cmds;
}

// -------------------------------------------------------------------------- //
template <>
inline
std::vector<CmdFileReader::Seconds>
CmdFileReader::calculate_run_durations(std::vector<C1Qcal> const & cmds) {

    std::vector<Seconds> run_durations;

    for (auto const & cal : cmds) {

        auto run_duration_seconds = Seconds( cal.settling_time() +
                                             cal.cal_duration() +
                                             cal.trailer_time() );

        run_durations.push_back(run_duration_seconds);

    }

    return run_durations;
}


// -------------------------------------------------------------------------- //
inline
void set_control_lines_a(C1Ssc & cmd_ssc,
                         int const line_number,
                         BmSensorControlOutput::Line const enable_line,
                         bool const active_high) {

    switch (line_number) {

        case 1 : {
            cmd_ssc.sensor_output_1a.line(enable_line);
            cmd_ssc.sensor_output_1a_active_high(active_high);
            break;
        }
        case 2 : {
            cmd_ssc.sensor_output_2a.line(enable_line);
            cmd_ssc.sensor_output_2a_active_high(active_high);
            break;
        }
        case 3 : {
            cmd_ssc.sensor_output_3a.line(enable_line);
            cmd_ssc.sensor_output_3a_active_high(active_high);
            break;
        }
        case 4 : {
            cmd_ssc.sensor_output_4a.line(enable_line);
            cmd_ssc.sensor_output_4a_active_high(active_high);
            break;
        }
        default : {
            throw WarningException("CmdFileReader",
                                   "construct_cmds",
                                   "input lines are [1, 2, 3, 4]");
        }
    }
}

// -------------------------------------------------------------------------- //
inline
void set_control_lines_b(C1Ssc & cmd_ssc,
                         int const line_number,
                         BmSensorControlOutput::Line const enable_line,
                         bool const active_high) {

    switch (line_number) {

        case 1 : {
            cmd_ssc.sensor_output_1b.line(enable_line);
            cmd_ssc.sensor_output_1b_active_high(active_high);
            break;
        }
        case 2 : {
            cmd_ssc.sensor_output_2b.line(enable_line);
            cmd_ssc.sensor_output_2b_active_high(active_high);
            break;
        }
        case 3 : {
            cmd_ssc.sensor_output_3b.line(enable_line);
            cmd_ssc.sensor_output_3b_active_high(active_high);
            break;
        }
        case 4 : {
            cmd_ssc.sensor_output_4b.line(enable_line);
            cmd_ssc.sensor_output_4b_active_high(active_high);
            break;
        }
        default : {
            throw WarningException("CmdFileReader",
                                   "construct_cmds",
                                   "input lines are [1, 2, 3, 4]");
        }
    }
}

// -------------------------------------------------------------------------- //
template <>
inline
std::vector<C1Ssc>
CmdFileReader::construct_cmds(UserInstruction const & ui,
                              TargetAddress const & ta) {

    std::ifstream outputs_fs;

    auto const runtime_config_path = Utility::get_runtime_config_path();
    std::string const sc_control_lines{"/sensor_control_lines.json"};

    outputs_fs.open(runtime_config_path + sc_control_lines);

    if (not outputs_fs) {
        throw WarningException("CmdFileReader",
                                "construct_cmds<C1Ssc>",
                                "can't open sc file: " + sc_control_lines);
    }

    std::stringstream outputs_ss;
    outputs_ss << outputs_fs.rdbuf();

    Json outputs_json = Json::parse( outputs_ss.str() );

    check_json(outputs_json.is_object(), "sc not in json format");

    // is the expected sc on the sc list?
    auto const & s = sn_.s_const_ref(ta);

    check_json( outputs_json.find( s.config.model.c_str() )
               != outputs_json.end(),
                std::string("sensor (") + s.config.model +
                std::string(") not found in sc file") );

    // setup complete, now to read the parsed configuration file
    Utility::JsonRef sensor_outputs_json =
        outputs_json[s.config.model.c_str()];

    // just one needed
    // ---------------------------------------------------------------------- //
    std::vector<C1Ssc> cmds{ C1Ssc() };
    using BSCL = BmSensorControlOutput::Line;
    bool center_or_cal_found = false;

    // center
    // --------------------------------------------------------------------- //
    if ( sensor_outputs_json.has("center") ) {

        center_or_cal_found = true;
        auto const center_output_json = sensor_outputs_json["center"];
        Utility::JsonRef const center_output = center_output_json;
        int const line_number = center_output["line"];
        bool const active_high = center_output["active_high"];

        if (s.config.input == Sensor::Input::a) {
            auto const enable_line = BSCL::sensor_a_centering;
            set_control_lines_a(cmds[0], line_number, enable_line, active_high);
        } else  {
            auto const enable_line = BSCL::sensor_b_centering;
            set_control_lines_b(cmds[0], line_number, enable_line, active_high);
        }
    }

    // cal
    // --------------------------------------------------------------------- //
    if ( sensor_outputs_json.has("cal") ) {

        center_or_cal_found = true;
        auto const cal_output_json = sensor_outputs_json["cal"];
        Utility::JsonRef const cal_output = cal_output_json;
        int const line_number = cal_output["line"];
        bool const active_high = cal_output["active_high"];

        if (s.config.input == Sensor::Input::a) {
            auto const enable_line = BSCL::sensor_a_calibration;
            set_control_lines_a(cmds[0], line_number, enable_line, active_high);
        } else  {
            auto const enable_line = BSCL::sensor_b_calibration;
            set_control_lines_b(cmds[0], line_number, enable_line, active_high);
        }
    }

    // for now only center or cal, aux1, aux2, etc can be added
    // --------------------------------------------------------------------- //
    if (not center_or_cal_found) {
        throw WarningException("CmdFileReader",
                               "construct_cmds<C1Ssc>",
                               "no center or cal found");
    }

    return cmds;
}

} // <- mzn

#endif // _MZN_CMD_FILE_READER_H_

