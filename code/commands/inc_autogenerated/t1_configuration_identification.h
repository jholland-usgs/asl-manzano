// ** T1ConfigurationIdentification Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_T1_CONFIGURATION_IDENTIFICATION_H
#define _MZN_CMD_T1_CONFIGURATION_IDENTIFICATION_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class T1ConfigurationIdentification : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, T1ConfigurationIdentification const & cmd);

public:

    explicit T1ConfigurationIdentification();
    ~T1ConfigurationIdentification() = default;

    T1ConfigurationIdentification(T1ConfigurationIdentification && rhs) = default;
    T1ConfigurationIdentification & operator=(T1ConfigurationIdentification && rhs) = default;
    T1ConfigurationIdentification(T1ConfigurationIdentification const & rhs) = default;
    T1ConfigurationIdentification & operator=(T1ConfigurationIdentification const & rhs) = default;
    std::string const cmd_name = "t1_configuration_identification";

    CmdFieldArrayChar<2> configuration_stream_seed_location;
    CmdFieldArrayChar<3> configuration_stream_seed_name;
    BmConfigurationIdentificationFlags flags;
    CmdFieldDuration<uint16_t, std::ratio<60> > interval;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, T1ConfigurationIdentification const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_T1_CONFIGURATION_IDENTIFICATION_H