// *%* CmdField Class ** //
// Manzano software
#include "cmd_field_pstring.h"
namespace mzn {

template <std::size_t N>
CmdFieldPstring<N>::CmdFieldPstring() :
        CmdFieldArray<typename data_type::value_type, N>{} {};

//! forward declaration
template class CmdFieldPstring<2>;
template class CmdFieldPstring<3>;
template class CmdFieldPstring<4>;
template class CmdFieldPstring<5>;
template class CmdFieldPstring<6>;
template class CmdFieldPstring<8>;
template class CmdFieldPstring<10>;
template class CmdFieldPstring<12>;
template class CmdFieldPstring<14>;

}// << mzn

