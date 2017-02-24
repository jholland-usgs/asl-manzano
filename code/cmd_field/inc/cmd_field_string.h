// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_STRING_H
#define _MZN_CMD_FIELD_STRING_H

#include "cmd_field.h"

namespace mzn {

//! TODO: all of this, need runtime checks or a constexpr string implementation
//! TODO: string_view might work, the strings are mostly view only
/*! @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
template <std::size_t N>
class CmdFieldString : public CmdField<std::string, N> {

public:

    using data_type = typename std::string;
    using M = typename CmdField<data_type>::M;

    //! initializes base
    explicit
    CmdFieldString();

    ~CmdFieldString() = default;

    //! same signature as CmdField
    std::size_t msg_to_data(M const & msg, std::size_t const mf_pos);
    std::size_t data_to_msg(M & msg, std::size_t const mf_pos) const;

    data_type operator()() const;
    void operator()(data_type const & in_data);
};

// constructor
// -------------------------------------------------------------------------- //
template <std::size_t N>
inline
CmdFieldString<N>::CmdFieldString() :
        CmdField<data_type, N>{} {

    this->data_.resize(N);
};

// -------------------------------------------------------------------------- //
template <std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldString<osN> const & cf) {
    cf_os << cf.data();
    return cf_os;
}

// once std::string is satisfies ContiguousContainer in C++17
// then reinterpret_cast can be used safely
// or use std::copy_n with parallel execution policy?
// currently all the strings are really small
// -------------------------------------------------------------------------- //
template <std::size_t N>
std::size_t CmdFieldString<N>::msg_to_data(M const & msg,
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
template <std::size_t N>
std::size_t CmdFieldString<N>::data_to_msg(M & msg,
                                           std::size_t const mf_pos) const {
    auto data_index = 0;
    for (auto i = mf_pos; i < mf_pos + N; i++) {
        msg[i] = static_cast<typename M::value_type>(this->data_[data_index]);
        data_index++;
    }

    return mf_pos + N;
}

// -------------------------------------------------------------------------- //
template <std::size_t N>
std::string CmdFieldString<N>::operator()() const {
    return this->data_;
}

// -------------------------------------------------------------------------- //
template <std::size_t N>
void CmdFieldString<N>::operator()(std::string const & in_data) {

    if ( in_data.size() != this->data_.size() )
        throw WarningException("CmdFieldString",
                               "operator()",
                               "string assignment should have same size");
    this->data_ = in_data;
}

// -------------------------------------------------------------------------- //
// template specialization for size known at run time
// -------------------------------------------------------------------------- //
template <>
std::size_t CmdFieldString<0>::msg_to_data(M const & msg,
                                           std::size_t const mf_pos) {
    auto const N = this->data_.size();

    // needs to be checked here since it is not known at compile time
    if ( msg.size()  < N + mf_pos) {
        throw WarningException(
            "CmdFieldString",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_pos " + std::to_string(mf_pos)
            + ", N " + std::to_string(N)
        );
    }

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
template <>
std::size_t CmdFieldString<0>::data_to_msg(M & msg,
                                           std::size_t const mf_pos) const {
    auto const N = this->data_.size();

    // needs to be checked here since it is not known at compile time
    if ( msg.size()  < N + mf_pos) {
        throw WarningException(
            "CmdFieldString",
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
std::string CmdFieldString<0>::operator()() const {
    return this->data_;
}

// -------------------------------------------------------------------------- //
template <>
void CmdFieldString<0>::operator()(std::string const & in_data) {
    this->data_ = in_data;
}
} // << mzn

#endif
