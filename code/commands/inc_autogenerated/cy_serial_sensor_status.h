// ** CySerialSensorStatus Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_CY_SERIAL_SENSOR_STATUS_H
#define _MZN_CMD_CY_SERIAL_SENSOR_STATUS_H

#include "command.h"


#include "cz_internal_temperature_measurement.h"
#include "cz_humidity_and_external_temperature.h"

#define k_cz_internal_temperature_measurement 0
#define k_cz_humidity_and_external_temperature 1

#include "multi_command_map.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class CySerialSensorStatus : public MultiCommandMap {

friend std::ostream & operator<<(std::ostream & cmd_os, CySerialSensorStatus const & cmd);

public:

    explicit CySerialSensorStatus();
    ~CySerialSensorStatus() = default;

    CySerialSensorStatus(CySerialSensorStatus && rhs) = default;
    CySerialSensorStatus & operator=(CySerialSensorStatus && rhs) = default;
    std::string const cmd_name = "cy_serial_sensor_status";

    CmdField<uint16_t> size_of_this_sub_block;
    CmdField<uint16_t> sensor_type;
    CmdField<uint16_t> serial_interface;
    CmdFieldDuration<uint16_t> seconds_per_sample;
    BmSerialsensorUnits units;
    CmdFieldDuration<int16_t, std::milli> integration_time;
    CmdField<uint16_t> fractional_digits;
    BmSerialsensorValidfields valid_fields;
    CmdField<int16_t> pressure_measurements;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;
    bool command_active(uint8_t const cmd_key) const override;
    // max keys known at compile time for fixed maps mc
    uint16_t number_of_ic(std::vector<uint8_t> const & msg,
                          uint16_t mf_begin) const override {
        return 2;
    }

    void create_new_ic(uint8_t const  cmd_key) override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, CySerialSensorStatus const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_CY_SERIAL_SENSOR_STATUS_H
