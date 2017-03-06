// Manzano software
#ifndef _MZN_TOKEN_H
#define _MZN_TOKEN_H

#include "command_container.h"


namespace mzn {

class Token : public CommandContainer {

//! Base for all commands that contains other inner commands
/*!
    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
public:

    Token(uint16_t const cmd_number,
          uint16_t const cmd_data_size);

    ~Token() = default;
    Token(Token && rhs) noexcept;
    Token & operator=(Token && rhs) noexcept;

protected:

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

    //! does not apply
    uint16_t number_of_ic(std::vector<uint8_t> const & msg,
                          uint16_t mf_begin) const override {

        throw WarningException("Token",
                               "number_of_ic",
                               "Token does not calculate nic");
    }

};


} // << mzn
#endif // _MZN_TOKEN_H_
