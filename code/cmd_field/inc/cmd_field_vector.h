// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_VECTOR_H
#define _MZN_CMD_FIELD_VECTOR_H

#include "cmd_field.h"
#include "string_utilities.h"

namespace mzn {

//! TODO: all of this, need runtime checks or a constexpr string implementation
//! TODO: string_view might work, the strings are mostly view only
//! could actually apply to vector too, the only difference seems to be
//! operator<<
/*! @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
template <std::size_t N>
class CmdFieldVector : public CmdField<std::vector<uint8_t>, N> {

public:

    using data_type = typename std::vector<uint8_t>;
    using M = typename CmdField<data_type>::M;

    //! initializes base
    explicit
    CmdFieldVector();

    ~CmdFieldVector() = default;

    //! same signature as CmdField
    std::size_t msg_to_data(M const & msg, std::size_t const mf_pos);
    std::size_t data_to_msg(M & msg, std::size_t const mf_pos) const;

    data_type operator()() const;
    void operator()(data_type const & in_data);
};

//! constructor
// -------------------------------------------------------------------------- //
template <std::size_t N>
inline
CmdFieldVector<N>::CmdFieldVector() :
        CmdField<data_type, N>{} {

    this->data_.resize(N);
};

// -------------------------------------------------------------------------- //
template <std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldVector<osN> const & cf) {
    auto const & v = cf.data();
    Utility::print_vector(v, cf_os);
    return cf_os;
}

// once std::vector<uint8_t> is satisfies ContiguousContainer in C++17
// then reinterpret_cast can be used safely
// or use std::copy_n with parallel execution policy?
// currently all the strings are really small
// -------------------------------------------------------------------------- //
template <std::size_t N>
inline
std::size_t CmdFieldVector<N>::msg_to_data(M const & msg,
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
inline
std::size_t CmdFieldVector<N>::data_to_msg(M & msg,
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
inline
std::vector<uint8_t> CmdFieldVector<N>::operator()() const {
    return this->data_;
}

// -------------------------------------------------------------------------- //
template <std::size_t N>
inline
void CmdFieldVector<N>::operator()(data_type const & in_data) {

    if (in_data.size() != N) {
        throw WarningException("CmdFieldVector",
                               "operator()",
                               "string assignment should have same size");
    }

    this->data_ = in_data;
}

// -------------------------------------------------------------------------- //
// template specialization for size known at run time
// -------------------------------------------------------------------------- //
template <>
inline
std::size_t CmdFieldVector<0>::msg_to_data(M const & msg,
                                           std::size_t const mf_pos) {

    // TODO add a third argument for count? hard to generalize
    // could be default to std::npos and then a check, for now is always
    // to the end of the message. If done it should be everywhere.
    int const N = this->data_.size() - mf_pos;

    // needs to be checked here since it is not known at compile time
    // N == 0 is ok, just resize data, skips for loop
    if (N < 0) throw WarningException( "CmdFieldVector",
                                       "msg_to_data",
                                       "msg size " + std::to_string( msg.size() )
                                       + ", mf_pos " + std::to_string(mf_pos)
                                       + ", N " + std::to_string(N) );

    data_.resize(N);

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
inline
std::size_t CmdFieldVector<0>::data_to_msg(M & msg,
                                           std::size_t const mf_pos) const {
    auto const N = this->data_.size();

    // needs to be checked here since it is not known at compile time
    if (msg.size()  < N + mf_pos) {
        throw WarningException(
            "CmdFieldVector",
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
std::vector<uint8_t> CmdFieldVector<0>::operator()() const {
    return this->data_;
}

// -------------------------------------------------------------------------- //
template <>
inline
void CmdFieldVector<0>::operator()(data_type const & in_data) {
    this->data_ = in_data;
}
} // << mzn

#endif
