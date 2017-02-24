// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_PSTRING_H
#define _MZN_CMD_FIELD_PSTRING_H

#include "cmd_field_array.h"

namespace mzn {

//! Overloads operator(), operator<<
/*!
    Handles pascal strings
    TODO: operator() std::string to/from pascal string
    @author rfigueroa@usgs.gov
    @throws logic setting operator() with wrong value
 */
// -------------------------------------------------------------------------- //
template <std::size_t N>
class CmdFieldPstring : public CmdFieldArray<uint8_t, N> {

public:

    using data_type = typename CmdFieldArray<uint8_t, N>::data_type;
    using CmdField<data_type>::data_;

    //! constructs base
    explicit
    CmdFieldPstring();

    ~CmdFieldPstring() = default;
};

// -------------------------------------------------------------------------- //
template <std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldPstring<osN> const & cf) {

    if (cf.data_.size() < 2) return cf_os;

    if ( (cf.data_[0] + 1) > cf.data_.size() ) {
        std::stringstream ss;
        for (auto const & b : cf.data_) ss << b << ",";
        ss << "@CmdFieldPstring pstring wrong  format";
        throw std::logic_error( ss.str() );
    }

    // pascal string have its size on the first byte
    for (int i = 1; i <= cf.data_[0]; i++ ) cf_os << cf.data_[i];

    return cf_os;
}

} // << mzn

#endif
