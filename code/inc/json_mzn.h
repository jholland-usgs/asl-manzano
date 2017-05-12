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
inline
Json read_json(std::string const & filename) {

    std::ifstream config_fs;

    config_fs.open(filename);

    if (not config_fs) {
        std::stringstream ss;
        ss << "Can't open configuration file(" << std::endl;
        ss << filename << ")" << std::endl;
        throw FatalException("Utility",
                             "read_json",
                             ss.str() );
    }

    // read in the contents of the entire file
    std::stringstream config_ss;
    config_ss << config_fs.rdbuf();

    // parses the read-in file as a json object
    auto const cals_json = Json::parse( config_ss.str() );

    if ( not cals_json.is_object() ) {
        std::stringstream ss;
        ss << "Not in json format(" << std::endl;
        ss << filename << ")" << std::endl;
        throw FatalException("Utility",
                             "read_json",
                             ss.str() );
    }

    return cals_json;
}

// -------------------------------------------------------------------------- //
template <typename T>
void ask(Json & json,
         std::string const & key,
         std::string const & hint) {

    std::cout << "\n" << key;
    if (hint != "") std::cout << " (" << hint << ")";
    std::cout << ": ";

    T value;
    // dont accept enter as response, wait until there is some response
    std::cin >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    if ( std::cin.fail() ) {
        std::cin.clear();
        std::stringstream ss;
        ss << "error with user response for key " << key;
        throw WarningException( "JsonSn", "ask", ss.str() );
    }

    json[key] = value;
}

// -------------------------------------------------------------------------- //
template <typename T>
void ask(Json & json,
         Json const & original_json,
         std::string const & key,
         std::string const & hint) {

    std::cout << "\n" << key;
    if (hint != "") std::cout << " (" << hint << ")";
    std::cout << "[" << original_json[key] << "]";
    std::cout << ": ";

    // accept enter as response
    std::string input;
    std::getline(std::cin, input);

    if ( std::cin.fail() ) {
        std::cin.clear();
        std::stringstream ss;
        ss << "error with user response for key " << key;
        throw WarningException( "JsonSn", "ask", ss.str() );
    }

    if (input == "") {
        json[key] = original_json[key];
        return;
    }

    if (std::is_same<T, std::string>::value) json[key] = input;
    else if (std::is_same<T, int>::value) json[key] = std::stoi(input);
    else throw std::logic_error("@JsonSn, ask, wrong type");
}

// -------------------------------------------------------------------------- //
inline
bool ask_yes(std::string const & prompt, bool const original = false) {

    std::cout << std::endl << prompt << "? ";
    std::cout << "(y/n) ";
    if (original == true) std::cout << "[true]";
    std::cout << ": ";

    std::string input;

    if (original == true) {
        // accept enter as response
        std::getline(std::cin, input);
    } else {
        // wait for actual response, enter will not skip this
        std::cin >> input;
    }

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
