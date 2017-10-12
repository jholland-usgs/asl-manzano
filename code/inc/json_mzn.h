#ifndef _MZN_JSON_MZN_H_
#define _MZN_JSON_MZN_H_

// to get environment variable
#include <string>
#include <exception>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <type_traits>

#include "mzn_cmake_config.h"
#include "mzn_except.h"
#include "json.h"
#include "string_utilities.h"
#include "system_calls.h"

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
        throw FatalException("Utility", "read_json", ss.str() );
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
        throw FatalException("Utility", "read_json", ss.str() );
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

    T input = Utility::checked_cin<T>();
    Utility::capitalize_alpha<T>(input);
    json[key] = input;
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

    bool gave_input;
    T input = Utility::checked_cin<T>(gave_input);
    if (gave_input) {
        Utility::capitalize_alpha<T>(input);
        json[key] = input;
    } else json[key] = original_json[key];
}

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
inline
std::vector<std::string> json_keys(std::string const & filename) {
    std::vector<std::string> keys;
    auto const runtime_config_path = get_runtime_config_path();
    auto const file_path = runtime_config_path + std::string("/") + filename;
    auto const j = Utility::read_json(file_path);
    for (auto it = j.begin(); it != j.end(); ++it) keys.push_back( it.key() );
    std::sort( keys.begin(), keys.end() );
    return keys;
}

// -------------------------------------------------------------------------- //
inline
std::string json_keys_str(std::vector<std::string> const & keys) {
    std::stringstream ss;
    for (auto const & key : keys) ss << "\n    " << key;
    ss << "\n           ";
    return ss.str();
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
            throw FatalException("Utility", "JsonRef[]", ss.str() );
        } else return json[key];
    }

    bool has(std::string  const key) const {
        if ( json.find(key) == json.end() ) return false;
        else return true;
    }
};


} // <- Utility
} // <- mzn

#endif
