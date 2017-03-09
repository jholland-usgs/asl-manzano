// *%* CmdField Class ** //
// Manzano software
#include "cmd_field_cdse.h"
namespace mzn {

// -------------------------------------------------------------------------- //
CmdFieldCdse::CmdFieldCdse() : CmdField<uint8_t>{} {};

// -------------------------------------------------------------------------- //
CmdFieldCdse::Element element_(uint8_t const data) {

    auto constexpr e_mask = 0b11000000;
    auto const e_number = data & e_mask;
    auto const e = static_cast<CmdFieldCdse::Element>(e_number);
    return e;
}

// -------------------------------------------------------------------------- //
CmdFieldCdse::LogicalOperator element_logical_operator_(uint8_t const data) {

    using LO = CmdFieldCdse::LogicalOperator;
    // these 2 happen to have the same value
    auto constexpr ev_mask = 0b00111111;
    auto constexpr eol_number = static_cast<uint8_t>(LO::end_of_list);

    auto const elo_number = data & ev_mask;

    if (elo_number > 5 and elo_number != eol_number ) {
        throw WarningException("CmdFieldCdse",
                               "element_logical_operator",
                               "value outside of enum range");
    }

    auto const elo = static_cast<LO>(elo_number);
    return elo;
}

// -------------------------------------------------------------------------- //
uint8_t element_value_(uint8_t const data) {

    auto const e = element_(data);
    using Element = CmdFieldCdse::Element;

    switch (e) {

        case Element::comm_event: {
            auto constexpr ev_mask = 0b00011111;
            auto const ev_number = data & ev_mask;
            return ev_number;
        }

        case Element::murdock_hutt_or_threshold_detector: {
            auto constexpr ev_mask = 0b00111111;
            auto const ev_number = data & ev_mask;
            return ev_number;
        }

        case Element::calibration_on: {
            auto constexpr ev_mask = 0b00111111;
            auto const ev_number = data & ev_mask;
            return ev_number;
        }

        case Element::logical_operator: {
            auto const elo = element_logical_operator_(data);
            return static_cast<uint8_t>(elo);
        }
    }
}

// -------------------------------------------------------------------------- //
CmdFieldCdse::Element
CmdFieldCdse::element() const {return element_(data_);}
// -------------------------------------------------------------------------- //
uint8_t
CmdFieldCdse::element_value() const {return element_value_(data_);}
// -------------------------------------------------------------------------- //
CmdFieldCdse::LogicalOperator
CmdFieldCdse::element_logical_operator() const {
    return element_logical_operator_(data_);
}

// -------------------------------------------------------------------------- //
void CmdFieldCdse::operator()(Element const e, LogicalOperator const lo) {
    (*this)( static_cast<uint8_t>(e) + static_cast<uint8_t>(lo) );
}

// -------------------------------------------------------------------------- //
void CmdFieldCdse::operator()(Element const e, uint8_t const number) {
    (*this)(static_cast<uint8_t>(e) + number);
}

// -------------------------------------------------------------------------- //
void CmdFieldCdse::operator()(uint8_t const & in_data) {

    try {
        element_value_(in_data);
    } catch (std::exception & e) {
        throw WarningException( "CmdFieldCdse",
                                "operator()",
                                e.what() );
    }

    data_ = in_data;
}

}// << mzn

