#ifndef _MZN_STRING_UTILITIES_H_
#define _MZN_STRING_UTILITIES_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <chrono>
#include <cstdio>
#include <future>

#include "mzn_except.h"

namespace mzn {
namespace Utility {

// -------------------------------------------------------------------------- //
template<typename T>
inline
void print_vector(T const & v, std::ostream & os = std::cout) {
    os << "[";
    for (auto const & b : v) {
        os << static_cast<int>(b) << ", ";
    }
    os << "]";
}
/*
 _______________________________________________________________________
 | 01 02 03 04 | 0D 0E 0F 10 | 19 1A 1B 1C | 25 26 27 28 | 31 32 33 34 |
 | 05 06 07 08 | 11 12 13 14 | 1D 1E 1F 20 | 29 2A 2B 2C | 35 36       |
 | 09 0A 0B 0C | 15 16 17 18 | 21 22 23 24 | 2D 2E 2F 30 |             | */

//! streams binary data formatted in columns, use for debugging and vectors
// -------------------------------------------------------------------------- //
template<typename M>
inline
void stream_hex(M const & msg, std::ostream & os = std::cout) {
    std::string separator_lines(71, '_');
    std::string separator_spaces(71, '_');
    std::string separator = " " + separator_lines;
    std::string spaces = " " + separator_lines;
    auto constexpr bigcols = 5u;
    auto constexpr cols = 4u;
    auto constexpr total_cols = cols * bigcols;
    // extra rows
    auto const msg_size = msg.size();
    auto const rows_int = msg_size / total_cols;
    auto const rows = msg_size % total_cols == 0 ? rows_int : rows_int + 1;
    os << "\n" << separator;
    auto i = 0u;
    os << std::uppercase << std::hex;

    for (auto row = 0u; row < rows; row++) {
        os << "\n |";
        for (auto bigcol = 0u; bigcol < bigcols; bigcol++) {
            os << " ";
            for (auto col = 0u; col < cols; col++) {
                i = col;
                i += bigcol * rows * cols;
                i += row * cols;

                if (i < msg.size() ) {
                    auto b = msg[i];
                    os << std::setw(2) << std::setfill('0')
                              << static_cast<const int>(b) << " ";
                } else os << "   ";
            }
            os << "|";
        }
    }

    os << std::nouppercase << std::dec;
    os << "\n" << std::endl;
}

// -------------------------------------------------------------------------- //
inline
std::vector<std::string> get_tokens(std::string const & line,
                                    char const delimiter = ',') {
    std::vector<std::string> tokens;
    std::stringstream ss;
    ss << line;
    std::string token;

    while ( std::getline(ss, token, delimiter) ) {
        if ( not token.empty() ) tokens.push_back(token);
        token.clear();
    }

    return tokens;
}

// -------------------------------------------------------------------------- //
inline
void capitalize_tokens(std::vector<std::string> & tokens) {
    for (auto & token : tokens) for (auto & c: token) c = std::toupper(c);
}

// -------------------------------------------------------------------------- //
template <typename T>
inline
void capitalize_alpha(T & token) {}

// -------------------------------------------------------------------------- //
template <>
inline
void capitalize_alpha(std::string & token) {
    for (auto & c : token) if ( std::isalpha(c) ) c = std::toupper(c);
}

// -------------------------------------------------------------------------- //
template <typename T>
inline
T checked_cin() {
    // checks for expected type and allows the result to be constant
    // accept enter as response
    T token;
    std::cin >> token;
    if ( std::cin.fail() ) {
        std::cin.clear();
        throw WarningException( "Utility", "checked_cin", "wrong input type");
    }
    return token;
}

// -------------------------------------------------------------------------- //
template <typename T>
inline
T checked_cin(bool & gave_input) {
    // getline only works with string like objects
    std::string token;
    std::getline(std::cin, token);
    // is ok to input nothing
    if ( token.empty() ) {
        gave_input = false;
        return T{};
    }
    // use ss to reinterpret the user input into T instead std::string
    gave_input = true;
    std::stringstream ss;
    ss << token;
    T value;
    ss >> value;
    if ( ss.fail() ) {
        throw WarningException("Utility", "checked_cin", "wrong input type");
    }
    return value;
}

// -------------------------------------------------------------------------- //
inline
void expect_string(std::string const & expected,
                   std::string const & token) {

    if (token != expected) {
        std::stringstream ss;
        ss << "expected:" << expected << " got:" << token;
        throw std::logic_error{ss.str()};
    }
}

// used to underline the problematic token in the error message
// -------------------------------------------------------------------------- //
inline
std::string underline_error(std::string const & token, unsigned const e_index) {

    std::stringstream token_underlined;

    if (token.size() > e_index) {
        token_underlined << "\n    " << token
                         << "\n    " << std::string(e_index, '~')
                         << "^"
                         << std::string(token.size() - e_index - 1, '~');
    }

    return token_underlined.str();
}

// -------------------------------------------------------------------------- //
inline
unsigned match_positive_number(std::string const & token,
                               std::size_t & token_index) {

    std::string token_number;

    for (unsigned i = token_index; i < token.size(); i++) {
        if ( std::isdigit(token[i]) ) token_number.push_back(token[i]);
        else break;
    }

    if ( token_number.empty() ) {

        std::stringstream error_msg;

        error_msg << "expected positive number in token \'" << token << "\'\n";

        if (token.size() > 1) {
            error_msg << "at:" << underline_error(token, token_index);
        }

        throw WarningException("Manzano",
                               "match_positive_number",
                               error_msg.str() );
    }

    token_index += token_number.size();

    return std::stoi(token_number);
}

// -------------------------------------------------------------------------- //
inline
bool is_positive_number(std::string const & s) {

    if ( s.empty() ) return false;
    for (auto const & c : s) if ( not std::isdigit(c) ) return false;
    return true;
}

// -------------------------------------------------------------------------- //
template <typename Period>
using DurationInt = std::chrono::duration<unsigned, Period>;

// -------------------------------------------------------------------------- //
inline
std::chrono::seconds match_duration(std::string const & token) {


    auto const duration_pos = token.find('&');

    if (duration_pos == std::string::npos or
        duration_pos == token.size() - 1 ) {

        throw WarningException("Utility",
                               "match_duration",
                               "provide a waiting time");
    }

    std::size_t token_index = duration_pos + 1;

    auto match_duration_char = [&]() {

        if ( token_index >= token.size() ) {
            throw WarningException("Utility",
                                   "match_duration",
                                   "provide s, m or h with number");
        }

        char c;

        if (token.find("s", token_index) == token_index) c = 's'; else
        if (token.find("m", token_index) == token_index) c = 'm'; else
        if (token.find("h", token_index) == token_index) c = 'h';
        else throw WarningException("Utility",
                                    "match_duration",
                                    "s m h not found");

        token_index += 1;
        return c;
    };

    // return seconds only
    std::chrono::seconds d(0);

    // allow for composite 2m30s for example
    while (token_index < token.size() ) {

        auto const d_count = match_positive_number(token, token_index);
        auto const d_char = match_duration_char();

        switch (d_char) {
            case 's': {d += DurationInt<std::ratio<1>>(d_count);    break;}
            case 'm': {d += DurationInt<std::ratio<60>>(d_count);   break;}
            case 'h': {d += DurationInt<std::ratio<3600>>(d_count); break;}
        }
    }

    return d;
}


} // <- Utility
} // <- mzn

#endif // _MZN_STRING_UTILITIES_H_
