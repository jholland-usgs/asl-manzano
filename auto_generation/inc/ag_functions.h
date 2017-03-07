
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cctype>
#include <bitset>
#include <algorithm>

#include "json.h"

using Json = mzn::Json;
// -------------------------------------------------------------------------- //
std::string to_camel_case(std::string cn) {

    // First letter always to upper case
    cn[0] = std::toupper(cn[0]);

    // Next letters when an underscore is found
    std::size_t underscore_pos;
    while ( (underscore_pos = cn.find("_") ) != std::string::npos ) {
        // check for underscore at the end
        if (underscore_pos == cn.size() ) {
            std::cerr << "ERROR: cmd name ending in underscore";
            std::exit(EXIT_FAILURE);
        }

        // Delete underscore and change first letter after
        cn.erase(underscore_pos, 1);
        cn[underscore_pos] = std::toupper(cn[underscore_pos]);
    }

    return cn;
}

// -------------------------------------------------------------------------- //
bool json_has_key(Json const & json, std::string const & key) {
    return ( json.find(key) != json.end() );
}

// -------------------------------------------------------------------------- //
void mc_key_enums(Json const & cmds_json,
                  std::ofstream & c_fs,
                  std::string const & cmd_name,
                  int const knn) {

    auto & ic_json = cmds_json[cmd_name.c_str()]["inner_commands"];

    bool has_key_values;

    if (knn > 0) {

        c_fs << "\n";
        c_fs << "\n    enum class Keys {";

        has_key_values = json_has_key(ic_json[0], "key_value");
        bool also_has_key_values;

        for (int kni = 0; kni < knn; kni++) {
            also_has_key_values = json_has_key(ic_json[kni], "key_value");
            if (has_key_values != also_has_key_values) {
                throw std::logic_error(
                    "mc_key_enums: either all have key_value or none");
            }
        }

        for (int kni = 0; kni < knn; kni++) {

            std::string const key_name = ic_json[kni]["key_name"];

            c_fs << "\n        ";
            c_fs << key_name << " = ";

            if (has_key_values) {
                uint8_t const key_value = ic_json[kni]["key_value"];
                c_fs << (int)key_value;
            } else {
                c_fs << kni;
            }

            c_fs << ",";
        }

        c_fs << "\n    };";
    }

    // now all the keys in an vector, so that this enum can be iterated:
    if (knn > 0) {

        c_fs << "\n\n    std::vector<uint8_t> const all_keys = {"
             << "\n       ";

        for (int kni = 0; kni < knn; kni++) {

            c_fs << " ";

            if (has_key_values) {
                uint8_t const key_value = ic_json[kni]["key_value"];
                c_fs << (int)key_value;
            } else {
                c_fs << kni;
            }

            c_fs << ",";
        }

        c_fs << "\n    };";

        c_fs << "\n\n    std::vector<uint8_t> const"
             << " keys() const override {return all_keys;}";
    }
}

// -------------------------------------------------------------------------- //
void mc_key_includes(Json const & cmds_json,
                     std::ofstream & c_fs,
                     std::string const & cmd_name,
                     int const knn) {

    // Since the mc
    for (int kni = 0; kni < knn; kni++) {

        std::string const key_name =
            cmds_json[cmd_name.c_str()]["inner_commands"][kni]["key_name"];

        c_fs << "\n";
        c_fs << "#include \"" << key_name << ".h\"";
    }
}

// -------------------------------------------------------------------------- //
void cmd_custom_new_ic_map(Json const & cmds_json,
                           std::ofstream & c_fs,
                           std::string const & cmd_name,
                           int const knn) {

    for (int kni = 0; kni < knn; kni++) {

        std::string key_name =
            cmds_json[cmd_name.c_str()]["inner_commands"][kni]["key_name"];

        std::string key_class_name = to_camel_case(key_name); // copy

        c_fs << "\n        ";
        c_fs << "case CKE::" << key_name << " : {";
        c_fs << "\n";
        c_fs << "\n            ";
        c_fs << "inner_commands.push_back(";
        c_fs << "\n            ";
        c_fs << "    std::unique_ptr<Command>{";
        c_fs << "\n            ";
        c_fs << "         std::make_unique<" << key_class_name << ">() } );";

        // nullptr check done after all select case
        c_fs << "\n            ";
        c_fs << "break;";
        c_fs << "\n        ";
        c_fs << "}";
    }
}

// -------------------------------------------------------------------------- //
void cmd_custom_new_ic(Json const & cmds_json,
                       std::ofstream & c_fs,
                       std::string const & cmd_name,
                       int const knn) {

    if (knn == 0) throw std::logic_error("@cmd_custom_new_ic empty cmd map");

    std::string key_name =
        cmds_json[cmd_name.c_str()]["inner_commands"][0]["key_name"];

    std::string key_class_name = to_camel_case(key_name); // copy

    c_fs << "\n    ";
    c_fs << "inner_commands.push_back(";
    c_fs << "\n    ";
    c_fs << "    std::unique_ptr<Command>{ std::make_unique<";
    c_fs << key_class_name << ">() } );";

    c_fs << "\n";

    c_fs << "\n    if (inner_commands.back() == nullptr) {"
         << "\n        throw WarningException(\"" << key_class_name << "\","
         << "\n                               \"create_new_ic\","
         << "\n                               \"nullptr inner command\");"
         << "\n     }"
         << "\n";
}

// -------------------------------------------------------------------------- //
void custom_command_fields(Json const & cmds_json,
                           std::ofstream & c_fs,
                           std::string const & cmd_name,
                           int const cfn) {

    // go over all command fields
    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        std::string cf_name = cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];
        int N = cf_json["cf_size"];

        c_fs << "\n    ";

        // now write it as cmd_field in the command
        // except for bitmaps, which have the letters Bm in position 0
        // also except when the whole type is given
        if ( cf_type.find("Bm") == 0  or
             cf_type.find("CmdField") != std::string::npos ) {

            c_fs << cf_type << " " << cf_name << ";";
        } else {
            c_fs << "CmdField" << cf_type << ", " << N << "> " << cf_name << ";";
        }
    }
}

// -------------------------------------------------------------------------- //
void custom_cf_units(Json const & cmds_json,
                     std::ofstream & c_fs,
                     std::string const & cmd_name,
                     int const cfn) {

    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        std::string cf_name = cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];
        int N = cf_json["cf_size"];

        c_fs << ",\n    " << cf_name << "(";;

        if ( json_has_key(cf_json, "cf_units") ) {

            std::string cf_units = cf_json["cf_units"];
            c_fs << "\"" << cf_units << "\"";
        }

        c_fs << ")";
    }
}

// -------------------------------------------------------------------------- //
void custom_print(Json const & cmds_json,
                  std::ofstream & c_fs,
                  std::string const & cmd_name,
                  int const cfn,
                  std::string const & cmd_class_name) {

    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        // printing name
        std::string cf_name =cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];

        // dont print ignored cmd_field
        if ( cf_type.find("Ignore") == std::string::npos ) {

            c_fs << "\n\n"
                 << "    cmd_os << \"\\n"  << cf_name << ": \";"
                 << " cmd_os << " << cf_name << ";";
        }
    }
}

// -------------------------------------------------------------------------- //
void custom_msg_to_data(Json const & cmds_json,
                        std::ofstream & c_fs,
                        std::string const & cmd_name,
                        int const cfn) {

    // go over all command fields
    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        std::string cf_name = cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];
        int N = cf_json["cf_size"];

        c_fs << "\n    mf_begin = ";

        // when the type is to be ignored, just move the mf_begin counter
        if ( cf_type.find("Ignore") == std::string::npos ) {
            c_fs << cf_name << ".msg_to_data(msg, mf_begin);";
        } else {
            c_fs << "mf_begin + " << N << "; // ignore type";
        }

    }
};

// -------------------------------------------------------------------------- //
void custom_data_to_msg(Json const & cmds_json,
                        std::ofstream & c_fs,
                        std::string const & cmd_name,
                        int const cfn) {

    // go over all command fields
    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        std::string cf_name = cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];
        int N = cf_json["cf_size"];

        c_fs << "\n    mf_begin = ";

        // when the type is to be ignored, just move the mf_begin counter
        if ( cf_type.find("Ignore") == std::string::npos ) {
            c_fs << cf_name << ".data_to_msg(msg, mf_begin);";
        } else {
            c_fs << "mf_begin + " << N << "; // ignore type";
        }
    }
};

