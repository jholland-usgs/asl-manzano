// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_PSTRING_H
#define _MZN_CMD_FIELD_PSTRING_H

#include "cmd_field.h"

namespace mzn {

//! Overloads operator(), operator<<
/*!
    Handles pascal strings
    @author rfigueroa@usgs.gov
    @throws logic setting operator() with wrong value
 */
// -------------------------------------------------------------------------- //
template <uint8_t N>
class CmdFieldPstring : public CmdField<std::string,
                                        static_cast<std::size_t>(N)> {

public:

    using data_type = typename std::string;
    using M = typename CmdField<data_type>::M;

    //! constructs base
    explicit
    CmdFieldPstring();

    explicit
    CmdFieldPstring(std::string const & pstring);

    ~CmdFieldPstring() = default;

    //! same signature as CmdField
    std::size_t msg_to_data(M const & msg, std::size_t const mf_pos);
    std::size_t data_to_msg(M & msg, std::size_t const mf_pos) const;

    data_type operator()() const;
    void operator()(data_type const & in_data);
};

//! constructor
// -------------------------------------------------------------------------- //
template <uint8_t N>
inline
CmdFieldPstring<N>::CmdFieldPstring() :
        CmdField<data_type, static_cast<std::size_t>(N)>{} {

    this->data_.resize(N);
    // there is a template specialization for N == 0
    this->data_[0] = N - 1;
};

//! constructor with pstring
// -------------------------------------------------------------------------- //
template <uint8_t N>
inline
CmdFieldPstring<N>::CmdFieldPstring(std::string const & pstring) :
        CmdField<data_type, static_cast<std::size_t>(N)>{} {
    // use operator()
    (*this)(pstring);
}

// -------------------------------------------------------------------------- //
template <uint8_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldPstring<osN> const & cf) {

    // pascal strings have its size on the first byte
    if (cf.data().size() < 2) return cf_os;

    // start on index 1
    std::string const pstring = cf.data().substr(1);
    cf_os << pstring;

    return cf_os;
}

// -------------------------------------------------------------------------- //
template <uint8_t N>
inline
std::size_t CmdFieldPstring<N>::msg_to_data(M const & msg,
                                            std::size_t const mf_pos) {
    // no temporary objects, only cast
    auto data_index = 0;
    using DataType = typename data_type::value_type;

    for (auto i = mf_pos; i < mf_pos + N; i++) {
        this->data_[data_index] = static_cast<DataType>(msg[i]);
        data_index++;
    }

    return mf_pos + N;
}

// -------------------------------------------------------------------------- //
template <uint8_t N>
inline
std::size_t CmdFieldPstring<N>::data_to_msg(M & msg,
                                            std::size_t const mf_pos) const {
    auto data_index = 0;
    for (auto i = mf_pos; i < mf_pos + N; i++) {
        msg[i] = static_cast<typename M::value_type>(this->data_[data_index]);
        data_index++;
    }

    return mf_pos + N;
}

// -------------------------------------------------------------------------- //
template <uint8_t N>
inline
std::string CmdFieldPstring<N>::operator()() const {
    return this->data_;
}

// -------------------------------------------------------------------------- //
template <uint8_t N>
inline
void CmdFieldPstring<N>::operator()(std::string const & in_data) {

    if (in_data.size() != N) {
        throw WarningException("CmdFieldPstring",
                               "operator()",
                               "string assignment should have same size");
    }

    auto const pascal_size = static_cast<std::size_t>(in_data[0]);

    // pascal string invariant
    if ( (pascal_size + 1) != in_data.size() ) {
        std::stringstream ss;
        ss << in_data;
        ss << "\n@CmdFieldPstring pstring wrong  format";
        throw std::logic_error( ss.str() );
    }

    this->data_ = in_data;
}

// -------------------------------------------------------------------------- //
// template specialization for size known at run time
// -------------------------------------------------------------------------- //

//! constructor
// -------------------------------------------------------------------------- //
template <>
inline
CmdFieldPstring<0>::CmdFieldPstring() :
        CmdField<data_type, 0>{} {
    // only one value, the current pascal size of 0
    this->data_.append('\0' + std::string(""));
};

// -------------------------------------------------------------------------- //
template <>
inline
std::size_t CmdFieldPstring<0>::msg_to_data(M const & msg,
                                            std::size_t const mf_pos) {

    // when doing auto generation this command field should be indicated as
    // cf_size == 1 (TODO: add test)
    // since the pascal string assumption is that an empty string is "0", not ""
    // so cf_size minimum size of 1 is the right way, msg[mf_pos] will be ok.
    // just in case, it is not done with auto generation
    if (msg.size() < 1 + mf_pos) {
        throw WarningException(
            "CmdFieldPstring",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_pos " + std::to_string(mf_pos)
        );
    }

    // N is the new size to take, not the current one
    // the new size is the first byte of the pascal string + the first byte (1)
    uint8_t const N = msg[mf_pos] + 1;

    // needs to be checked here since it is not known at compile time
    if (msg.size() < N + mf_pos) {
        throw WarningException(
            "CmdFieldPstring",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_pos " + std::to_string(mf_pos)
            + ", N " + std::to_string(N)
        );
    }

    // no temporary objects, only cast
    auto data_index = 0;
    using DataType = typename data_type::value_type;

    // all good, resize data_ to fit msg
    this->data_.resize(N);

    for (auto i = mf_pos; i < mf_pos + N; i++) {
        this->data_[data_index] = static_cast<DataType>(msg[i]);
        data_index++;
    }

    return mf_pos + N;
}

// -------------------------------------------------------------------------- //
template <>
inline
std::size_t CmdFieldPstring<0>::data_to_msg(M & msg,
                                            std::size_t const mf_pos) const {
    auto const N = this->data_.size();

    // needs to be checked here since it is not known at compile time
    if (msg.size()  < N + mf_pos) {
        throw WarningException(
            "CmdFieldPstring",
            "data_to_msg",
            "msg size " + std::to_string( msg.size() )
            + ", mf_pos " + std::to_string(mf_pos)
            + ", N " + std::to_string(N)
        );
    }

    auto data_index = 0;
    for (auto i = mf_pos; i < mf_pos + N; i++) {
        msg[i] = static_cast<typename M::value_type>(this->data_[data_index]);
        data_index++;
    }

    return mf_pos + N;
}

// -------------------------------------------------------------------------- //
template <>
inline
std::string CmdFieldPstring<0>::operator()() const {
    return this->data_;
}

// -------------------------------------------------------------------------- //
template <>
inline
void CmdFieldPstring<0>::operator()(std::string const & in_data) {

    auto const pascal_size = static_cast<std::size_t>(in_data[0]);

    // pascal string invariant, includes valid string "0"
    // string "" is considered not valid and will fail
    if ( (pascal_size + 1) != in_data.size() ) {
        std::stringstream ss;
        ss << in_data;
        ss << "\n@CmdFieldPstring pstring wrong  format";
        throw std::logic_error( ss.str() );
    }

    this->data_ = in_data;
}

} // << mzn

#endif
