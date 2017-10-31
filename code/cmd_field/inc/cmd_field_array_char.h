// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_ARRAY_CHAR_H
#define _MZN_CMD_FIELD_ARRAY_CHAR_H

#include "cmd_field_array.h"

namespace mzn {

//! Array of chars, when intended to be printed as text (e.g. ascii) chars
//! @see CmdFieldArray
/*! @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
template <std::size_t N>
class CmdFieldArrayChar : public CmdFieldArray<char, N> {

public:

    using data_type = typename CmdFieldArray<char, N>::data_type;

    //! initializes base
    explicit
    CmdFieldArrayChar();

    //! constructor with different input_T
    template <typename input_T>
    explicit
    CmdFieldArrayChar(input_T const & input_msg) :
        CmdFieldArray<typename data_type::value_type, N>{} {
        auto msg = std::vector<uint8_t>( input_msg.begin(), input_msg.end() );
        this->msg_to_data(msg, 0);
    }

    ~CmdFieldArrayChar() = default;
};

// -------------------------------------------------------------------------- //
template <std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldArrayChar<osN> const & cf) {

    cf_os << "'";
    for (auto const & c : cf.data() ) cf_os << c;
    cf_os << "'";

    return cf_os;
}

// -------------------------------------------------------------------------- //
template <std::size_t N>
inline
CmdFieldArrayChar<N>::CmdFieldArrayChar() :
        CmdFieldArray<typename data_type::value_type, N>{} {};

} // << mzn

#endif
