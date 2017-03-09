// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_CDSE_H
#define _MZN_CMD_FIELD_CDSE_H

#include "cmd_field.h"

namespace mzn {

//! Overloads base operator(), operator<<
/*! used for cdse = Control Detector Specification (token 131) Equation
    each one of this commands is an element of that equation
    @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
class CmdFieldCdse : public CmdField<uint8_t> {
public:

    using data_type = uint8_t;
    using CmdField<data_type>::data;

    //! constructs base
    explicit
    CmdFieldCdse();

    ~CmdFieldCdse() = default;

    enum class Element {
        comm_event                         = 0b00000000,
        murdock_hutt_or_threshold_detector = 0b01000000,
        calibration_on                     = 0b10000000,
        logical_operator                   = 0b11000000,
    };

    enum class LogicalOperator {
        left_paren   = 0,
        right_paren  = 1,
        not_         = 2,
        and_         = 3,
        or_          = 4,
        xor_         = 5,
        end_of_list  = 0b00111111,
    };


    //! special accessor / mutators
    // ---------------------------------------------------------------------- //
    Element element() const;
    LogicalOperator element_logical_operator() const;
    uint8_t element_value() const;
    void operator()(data_type const & in_data);
    void operator()(Element const e, LogicalOperator const lo);
    void operator()(Element const e, uint8_t const number);
};

// operator<< overload
// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldCdse::LogicalOperator const & cf_lo) {

    using LO = CmdFieldCdse::LogicalOperator;

    switch (cf_lo) {
        case LO::left_paren  : {cf_os << "(";   break;}
        case LO::right_paren : {cf_os << ")";   break;}
        case LO::not_        : {cf_os << "not"; break;}
        case LO::and_        : {cf_os << "and"; break;}
        case LO::or_         : {cf_os << "or";  break;}
        case LO::xor_        : {cf_os << "xor"; break;}
        case LO::end_of_list : {cf_os << "end"; break;}
        default: throw std::logic_error("@CmdFieldCdse::operator<<");
    }

    return cf_os;
}


// operator<< overload
// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldCdse::Element const & cf_e) {

    using Element = CmdFieldCdse::Element;

    switch (cf_e) {
        case Element::comm_event  : {cf_os << "comm_event";   break;}

        case Element::murdock_hutt_or_threshold_detector  : {
            cf_os << "murdock_hutt_or_threshold_detector";
            break;
        }

        case Element::calibration_on  : {cf_os << "calibration_on";   break;}
        case Element::logical_operator : {cf_os << "logical_operator";   break;}

        default: throw std::logic_error("@CmdFieldCdse::Element::operator<<");
    }

    return cf_os;
}

// operator<< overload
// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldCdse const & cf) {

    using Element = CmdFieldCdse::Element;

    Element const e = cf.element();
    cf_os << e << "::";

    if (e == Element::logical_operator) {
        cf_os << cf.element_logical_operator();
    } else {
        cf_os << static_cast<int>( cf.element_value() );
    }

    return cf_os;
}

} // << mzn

#endif
