// ** T1ControlDetectorSpecification Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_T1_CONTROL_DETECTOR_SPECIFICATION_H
#define _MZN_CMD_T1_CONTROL_DETECTOR_SPECIFICATION_H

#include "command.h"


#include "tx_detector_equation.h"

#include "command_vector_nb.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class T1ControlDetectorSpecification : public CommandVectorNb {

friend std::ostream & operator<<(std::ostream & cmd_os, T1ControlDetectorSpecification const & cmd);

public:

    explicit T1ControlDetectorSpecification();
    ~T1ControlDetectorSpecification() = default;

    T1ControlDetectorSpecification(T1ControlDetectorSpecification && rhs) = default;
    T1ControlDetectorSpecification & operator=(T1ControlDetectorSpecification && rhs) = default;
    std::string const cmd_name = "t1_control_detector_specification";

    CmdField<uint8_t> number_of_bytes;
    CmdField<uint8_t> detector_number;
    BmControlDetectorSpecificationOption detector_option;
    CmdFieldPstring<0> detector_name;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;
    // max keys implementation in a separate function/file
    // not part of auto generation
    uint16_t nb(std::vector<uint8_t> const & msg,
                uint16_t mf_begin) const override;
    void create_new_ic(uint8_t const  cmd_key) override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, T1ControlDetectorSpecification const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_T1_CONTROL_DETECTOR_SPECIFICATION_H