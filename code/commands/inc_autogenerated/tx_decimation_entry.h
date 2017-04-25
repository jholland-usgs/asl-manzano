// ** TxDecimationEntry Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_TX_DECIMATION_ENTRY_H
#define _MZN_CMD_TX_DECIMATION_ENTRY_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class TxDecimationEntry : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, TxDecimationEntry const & cmd);

public:

    explicit TxDecimationEntry();
    ~TxDecimationEntry() = default;

    TxDecimationEntry(TxDecimationEntry && rhs) = default;
    TxDecimationEntry & operator=(TxDecimationEntry && rhs) = default;
    TxDecimationEntry(TxDecimationEntry const & rhs) = default;
    TxDecimationEntry & operator=(TxDecimationEntry const & rhs) = default;
    std::string const cmd_name = "tx_decimation_entry";

    CmdField<uint8_t> source_lcq_number;
    CmdField<uint8_t> decimation_fir_filter_number;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, TxDecimationEntry const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_TX_DECIMATION_ENTRY_H