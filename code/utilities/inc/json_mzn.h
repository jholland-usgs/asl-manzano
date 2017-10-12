#ifndef _MZN_JSON_MZN_H_
#define _MZN_JSON_MZN_H_

// to get environment variable
#include <string>
#include <exception>
#include <cstdlib>
#include <limits>
#include <type_traits>

#include "mzn_cmake_config.h"

#include "mzn_except.h"
#include "json.h"
#include "string_utilities.h"

namespace mzn {
namespace Utility {

// -------------------------------------------------------------------------- //
Json read_json(std::string const & filename) {
// -------------------------------------------------------------------------- //
template <typename T>
void ask(Json & json,
         std::string const & key,
         std::string const & hint);
// -------------------------------------------------------------------------- //
template <typename T>
void ask(Json & json,
         Json const & original_json,
         std::string const & key,
         std::string const & hint);
// -------------------------------------------------------------------------- //
inline
bool ask_yes(std::string const & prompt, bool const original = false) {

    std::cout << std::endl << prompt << "? ";
    std::cout << "(y/n) ";
    if (original == true) std::cout << "[true]";
    std::cout << ": ";

    std::string input;

    if (original == true) std::getline(std::cin, input);
    else std::cin >> input;

    if ( std::cin.fail() ) {
        std::cin.clear();
        std::stringstream ss;
        ss << "error with user response for " << prompt;
        throw WarningException( "JsonSn", "ask_yes", ss.str() );
    }

    if (input == "" and original == true) return true;

    if (input == "y") return true; else return false;
}

// -------------------------------------------------------------------------- //
class JsonRef {

public:
    // internal object
    Json const & json;
    // implicit conversion
    JsonRef(Json const & json) : json(json) {}

    auto operator[](std::string const key) const {

        if ( json.find(key) == json.end() ) {

            std::stringstream ss;
            ss << "Error attempting to get value for key: " << key;
            throw FatalException("Utility",
                                 "JsonRef[]",
                                 ss.str() );
        } else {
            return json[key];
        }
    }

    bool has(std::string  const key) const {
        if ( json.find(key) == json.end() ) return false;
        else return true;
    }
};


} // <- Utility
} // <- mzn

#endif
