#include "cmd_file_reader.h"

namespace mzn {

// -------------------------------------------------------------------------- //
CmdFileReader::CmdFileReader(SeismicNetwork const & sn) : sn_(sn) {}

// -------------------------------------------------------------------------- //
void CmdFileReader::check_json(bool const check, std::string const & e_what) {

    if (not check) throw WarningException("CmdFileReader",
                                          "construct_cals",
                                          e_what);
}

// -------------------------------------------------------------------------- //
std::vector<CmdFileReader::Seconds>
CmdFileReader::calculate_delays(std::vector<Seconds> const & run_durations) {

    // --------- delays ----------- //
    // wiggle seconds might not really be needed
    // gives time to the digitizer so that you avoid the error
    // calibration in progress
    // but calibrations will look better with some separation
    // and program will be more robust to system delays (this is not real time)
    // ideally all cals should be sent to md_ before the first cal starts

    std::vector<Seconds> delays;
    delays.reserve( run_durations.size() );

    Seconds constexpr wiggle_seconds(1);
    Seconds delay_seconds(0);

    for (auto const & run_duration : run_durations) {
        delays.push_back(delay_seconds);
        // delay next message to fit after this calibration
        delay_seconds += wiggle_seconds + run_duration;
    }

    return delays;
}

// -------------------------------------------------------------------------- //
void set_center_a(C1Ssc & cmd_ssc, int const line, bool const active_high) {

    auto const center_line = BmSensorControlOutput::Line::sensor_a_centering;

    switch (line) {

        case 1 : {
            cmd_ssc.sensor_output_1a.line(center_line);
            cmd_ssc.sensor_output_1a_active_high(active_high);
            break;
        }
        case 2 : {
            cmd_ssc.sensor_output_2a.line(center_line);
            cmd_ssc.sensor_output_2a_active_high(active_high);
            break;
        }
        case 3 : {
            cmd_ssc.sensor_output_3a.line(center_line);
            cmd_ssc.sensor_output_3a_active_high(active_high);
            break;
        }
        case 4 : {
            cmd_ssc.sensor_output_4a.line(center_line);
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

} // <- mzn
