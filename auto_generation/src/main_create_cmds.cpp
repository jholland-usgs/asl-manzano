// Uses the quanterra command format
// to create classes which can be then used for serialization
// into quanterra data packets

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cctype>
#include <bitset>
#include <algorithm>

/*! This file autogenerates:
   - one class/file for each command in the cmd json file

   Commands data structures get serialized into messages.

   Messages received are de-serialized into a command data structure.

   A command is composed of cmd_field (cf) objects.

   Most commands have a fixed format and size, where the relationship between
   the message received in bytes and the command data structure in cmd_fields
   is known at compile time. Commands that have variable size are called
   command_container (mc) due that the variability comes from having inner
   commands (ic) embedded into the command_container message. For these commands,
   the relationship needs to be worked out at run time.

   The command data size refers to the number of bytes that a
   serialized message with all its cmd_field will need. For command_container this
   only includes the cmd_field in the command_container itself, not the ones in the
   inner commands.

   The digitizer also shows some weird cases. For example a 48 bit (6 bytes)
   integer. The underlaying data of the cmd_field would actually be 8 bytes
   since architectures do not feature 6 bytes ints.

   For command_container, the cmd_data_size_ is not known at compile time, many times
   is not known until the message for that command is received and the message
   body is analyzed. Due to this, the cmd_data_size_ is set to the mtu
   (maximum transferable unit) of the digitizers UPD connection.
*/

#include "json.h"
#include "ag_functions.h"

// -------------------------------------------------------------------------- //
int main() {

    using Json = mzn::Json;

    std::cout << std::endl << "commands class builder" << std::endl;

    // open and parse the JSON
    std::ifstream cmds_fs;
    cmds_fs.open("../cmds_format.json");
    if (not cmds_fs) {
        std::cerr << "ERROR: can't open commands format file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // read/parse the file into a Document with DOM format
    std::stringstream cmds_ss;
    cmds_ss << cmds_fs.rdbuf();
    Json cmds_json;
    cmds_json = Json::parse( cmds_ss.str() );

    // loop over the commands from the JSON
    // modifying the specific file for this type of command
    for (auto cmd_itr = cmds_json.begin(); cmd_itr != cmds_json.end(); cmd_itr++) {

        //! cmd_name: command being used in this iteration, e.g. c1_ping
        std::string cmd_name = cmd_itr.key();

        // each command is written in a cpp/h pair
        // first open/create the h file
        std::ofstream c_fs;
        c_fs.open("../../code/commands/inc_autogenerated/" + cmd_name + ".h",
                  std::ofstream::out | std::ofstream::trunc);

        // cmd in upper case for guards, e.g. C1_PING
        std::string cmd_name_uppercase = cmd_name; //copy in C++11
        for (auto & c: cmd_name_uppercase) c = std::toupper(c);

        // cmd in camel case for class name, e.g. C1Ping:
        std::string cmd_class_name = to_camel_case(cmd_name);

        // file comments:
        c_fs << "// ** " + cmd_class_name + " Class ** //\n";
        c_fs << "// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //\n";
        c_fs << "// Manzano Software //\n";

        // guards:
        c_fs << "\n#ifndef _MZN_CMD_" <<  cmd_name_uppercase << "_H\n";
        c_fs << "#define _MZN_CMD_" <<  cmd_name_uppercase << "_H\n";

        // manzano includes:
        c_fs << "\n#include \"command.h\"\n";

        std::cout << "Processing: " << cmd_class_name << std::endl;

        //! cfn = number of command fields
        int cfn = cmds_json[cmd_name.c_str()]["cmd_fields"].size();

        //! cmd_data_size = bytes of a serialized message containing all cmd data
        int cmd_data_size = 0;
        for (int cfi = 0; cfi < cfn; cfi++) {
            int cf_size =
                cmds_json[cmd_name.c_str()]["cmd_fields"][cfi]["cf_size"];
            cmd_data_size += cf_size;
        }

        // initializing variables to default, non mc values
        bool command_container = false;
        int mc_header_size = 0;
        bool fixed_map;
        bool info_ni;
        int mc_cmd_map_size = 0;

        // if mc, setup the mc values for the values in the json
        // also, add necessary #define, #include in the files
        if ( json_has_key(cmds_json[cmd_name.c_str()], "inner_commands") ) {

            command_container = true;

            // The command fields in this mc serve as the header of the inner commands
            mc_header_size = cmd_data_size;

            /*  Two types of multi commands (mc):
              - fixed_map: mc that have specific inner commands expected.
                Not all inner commands will be present but maximum number and order
                is known at compile time.
              - Not fixed_map: mc that have an unknown number of the same inner
                command. In practice the number is limited with the mtu
                (maximum transferable unit) in the q330 udp connection
              */

            std::string const container_type =
                cmds_json[cmd_name.c_str()]["container_type"];

            std::string const container_info =
                cmds_json[cmd_name.c_str()]["container_info"];

            // TODO: generalize
            fixed_map = (container_type == "map");
            // ni = number_of_items / nb = number_of_bytes
            info_ni = (container_info == "ni");

            // get number of defined inner commands:
            mc_cmd_map_size = cmds_json[cmd_name.c_str()]["inner_commands"].size();

            if ( (not fixed_map) and (mc_cmd_map_size != 1) ){
                // mc that are Not fixed maps should have one or none
                // defined inner command
                std::cerr << "ERROR: Multi commands should have one inner command"
                          << std::endl << "except for fixed maps";

                std::exit(EXIT_FAILURE);
            }

            // #include files for inner commands,
            // class enum the key names
            // ***** key names for map ******
            c_fs << "\n";
            mc_key_includes(cmds_json, c_fs, cmd_name, mc_cmd_map_size);
            c_fs << "\n";
        }

        std::string base_file;
        std::string base_class;
        std::string base_func;

        // class #include depencies and inheritance parents are different
        // from fixed_map and not fixed_map commands.
        // also different for mc and normal commands
        if (command_container) {

            // class declaration
            if (fixed_map) {
                base_file = "command_map";
                base_class = "CommandMap";
            } else {
                base_file = "command_vector";
                base_class = "CommandVector";
            }

            // class declaration
            if (info_ni) {
                base_file += "_ni";
                base_class += "Ni";
                base_func = "ni";
            } else {
                base_file += "_nb";
                base_class += "Nb";
                base_func = "nb";
            }

            c_fs << "\n#include \"" << base_file  << ".h\""
                 << "\nnamespace mzn {"
                 << "\n\n// -------------------------------------------"
                 << "------------------------------- //"
                 << "\nclass " << cmd_class_name
                 << " : public " << base_class << " {\n";
        } else {

            c_fs << "\nnamespace mzn {"
                 << "\n\n// ------------------------------------------------"
                 << "-------------------------- //"
                 << "\nclass " << cmd_class_name << " : public Command {\n";
        }

        // add operator<< as a friend to access protected member function os_print
        c_fs << "\nfriend std::ostream & operator<<(std::ostream & cmd_os, "
             << cmd_class_name << " const & cmd);";

        c_fs << "\n\npublic:";

        // Add definition for keys

        if (command_container and fixed_map) {

            // key enums for map
            c_fs << "\n\n    // ---------------------------------------"
                 << "------------------------------- //";
            mc_key_enums(cmds_json, c_fs, cmd_name, mc_cmd_map_size);
        }

        //TODO: default constructor
        c_fs << "\n\n    explicit " << cmd_class_name <<"();"; //default constructor
        c_fs << "\n    ~" << cmd_class_name <<"() = default;"; //default destructor

        c_fs << "\n\n    " << cmd_class_name << "("
             << cmd_class_name << " && rhs) = default;";

        c_fs << "\n    " << cmd_class_name << " & operator=(" << cmd_class_name
             << " && rhs) = default;";

        if (not command_container) {

            c_fs << "\n    " << cmd_class_name << "("
                 << cmd_class_name << " const & rhs) = default;";

            c_fs << "\n    " << cmd_class_name << " & operator=("
                 << cmd_class_name << " const & rhs) = default;";
        }

        // the object knows its code for debug, e.g. c1_stat
        c_fs << "\n    std::string const cmd_name = \"" << cmd_name << "\";\n";

        // ***** command fields ******
        custom_command_fields(cmds_json, c_fs, cmd_name, cfn);

        // ***** msg_to_data declaration ******
        c_fs << "\n\n    "
             << "uint16_t msg_to_data(std::vector<uint8_t> const & msg,"
             << "\n    "
             << "                     uint16_t mf_begin) override;";

        // ***** data_to_msg declaration ******
        c_fs << "\n\n    "
             << "uint16_t data_to_msg(std::vector<uint8_t> & msg,"
             << "\n    "
             << "                     uint16_t mf_begin) const override;";


        // function declarations for multi command
        if (command_container) {

            // command_active (* yes)
            //        |Ni|Nb|
            //        -------
            // vector |  |  |
            // map    |* |  |
            //        -------

            if (info_ni and fixed_map) {
                c_fs << "\n    bool command_active(uint8_t const cmd_key)"
                     << " const override;";
            }

            /*
            if (
                c_fs << "\n    // max keys known at compile time for fixed maps mc";
                c_fs << "\n    uint16_t " << base_func << "(std::vector<uint8_t> const & msg,";
                c_fs << "\n                uint16_t mf_begin) const override";
                c_fs << " {";
                c_fs << "\n        return " << mc_cmd_map_size << ";";
                c_fs << "\n    }";
                c_fs << "\n";

            }
            */

            // ni() or nb() (* yes)
            //        |Ni|Nb|
            //        -------
            // vector |* |* |
            // map    |  |* |
            //        -------

            if ( (not info_ni) or (not fixed_map) ) {

                c_fs << "\n    // max keys implementation"
                     << " in a separate function/file";

                c_fs << "\n    // not part of auto generation";
                c_fs << "\n    uint16_t " << base_func
                     << "(std::vector<uint8_t> const & msg,";

                c_fs << "\n                uint16_t mf_begin) const override";
                c_fs << ";";
            }

            // create_new_ic
            c_fs << "\n    void create_new_ic(uint8_t const  cmd_key) override;";
        }

        c_fs << "\n\nprivate:";

        // provide cmd_number for parent command

        // cmd_number is a string here, but will be a hexadecimal literal
        // on the command class being created
        std::string cmd_number = cmds_json[cmd_name.c_str()]["cmd_number"];

        // declaration of ostream for printing
        c_fs << "\n\n    std::ostream & os_print("
             << "std::ostream & cmd_os) const override;";

        // end of class declaration
        c_fs << "\n};\n";

        // overload of operator<<
        c_fs << "\n"
             << "\ninline std::ostream & operator<<(std::ostream & cmd_os, "
             << cmd_class_name << " const & cmd) {"
             << "\n    return cmd.os_print(cmd_os);"
             << "\n}\n";

        // guards end if
        c_fs << "\n} // end namespace"
             << "\n" << "#endif // _MZN_CMD_"
             << cmd_name_uppercase << "_H" << std::endl;

        // close .h
        c_fs.close();

        // ----- *********************** ----- //
        // ----- Change file to cpp file ----- //
        // ----- ***********************  ----- //

        // open .cpp
        c_fs.open("../../code/commands/src_autogenerated/" + cmd_name + ".cpp",
                  std::ofstream::out | std::ofstream::trunc);

        // include .h
        c_fs << "#include \"" << cmd_name << ".h\"\n"
             << "\nnamespace mzn {";

        // **************************************
        // class constructor definition
        // **************************************
        c_fs << "\n" << cmd_class_name << "::" << cmd_class_name << "():\n";

        if (command_container) {
            c_fs << "    " << base_class << "(";
        } else {
            c_fs << "    Command(";
        }

        c_fs << cmd_number << ", ";
        c_fs << cmd_data_size << ")";

        // provide constructor arguments

        // **************************************
        // cmd_field constructors with units
        // **************************************
        custom_cf_units(cmds_json, c_fs, cmd_name, cfn);

        c_fs << " { }\n"; // constructor closing bracket

        // ***** msg_to_data implementation ******

        std::string cmd_class_name_spaces(cmd_class_name.size(), ' ');

        c_fs << "\nuint16_t " << cmd_class_name << "::"
             << "msg_to_data(std::vector<uint8_t> const & msg,"
             << "\n         " << cmd_class_name_spaces << "  "
             << "            uint16_t mf_begin) {\n";

        if (cfn > 0) {
            c_fs << "\n    if ( msg.size()  < cmd_data_size_ + mf_begin) {"
                 << "\n        throw FatalException("
                 << "\n            \"" << cmd_class_name << "\","
                 << "\n            \"msg_to_data\","
                 << "\n            \"msg size \" + std::to_string( msg.size() )"
                 << "\n            + \", mf_begin \" + std::to_string(mf_begin)"
                 << "\n            + \", cmd_data_size_ \" + std::to_string(cmd_data_size_)"
                 << "\n        );"
                 << "\n    }\n";
        }

        custom_msg_to_data(cmds_json, c_fs, cmd_name, cfn);

        if (command_container) {
            c_fs << "\n    mf_begin = " << base_class << "::msg_to_data(msg, mf_begin);";
        }

        c_fs << "\n\n    return mf_begin;";
        c_fs << "\n}\n";


        // ***** data_to_msg implementation ******

        c_fs << "\nuint16_t " << cmd_class_name << "::"
             << "data_to_msg(std::vector<uint8_t> & msg,"
             << "\n         " << cmd_class_name_spaces << "  "
             << "            uint16_t mf_begin) const {\n";

        if (cfn > 0) {
            c_fs << "\n    if ( msg.size()  < cmd_data_size_ + mf_begin) {"
                 << "\n        throw FatalException("
                 << "\n            \"" << cmd_class_name << "\","
                 << "\n            \"msg_to_data\","
                 << "\n            \"msg size: \" + std::to_string( msg.size() )"
                 << "\n            + \", mf_begin \" + std::to_string(mf_begin)"
                 << "\n            + \", cmd_data_size_ \" + std::to_string(cmd_data_size_)"
                 << "\n        );"
                 << "\n    }\n";
        }

        custom_data_to_msg(cmds_json, c_fs, cmd_name, cfn);

        if (command_container) {

            c_fs << "\n    mf_begin = ";

            if (fixed_map and not info_ni) c_fs << "CommandMapNb";
            else c_fs << "CommandContainer";

            c_fs << "::data_to_msg(msg, mf_begin);";
        }

        c_fs << "\n\n    return mf_begin;";
        c_fs << "\n}\n";

        // custom print output to console
        c_fs << "\nstd::ostream & " << cmd_class_name << "::os_print(";
        c_fs << "std::ostream & cmd_os) const {";
        c_fs << "\n    cmd_os << ";
        c_fs << "\"\\n --- " << cmd_name_uppercase << " ---  \\n\";";

        custom_print(cmds_json, c_fs, cmd_name, cfn,
                     cmd_class_name);

        c_fs << "\n    cmd_os << std::endl;";

        // multi command custom print
        c_fs << "\n";
        if (command_container) {
            c_fs << "\n    return CommandContainer::os_print(cmd_os);";
        } else {
            c_fs << "\n    return cmd_os;";
        }

        c_fs << "\n}\n"; // end of printing

        // **************************************
        // multi command functions implementation
        // **************************************

        if (command_container) {
            c_fs << "\n";
            c_fs << "\nvoid "
                 << cmd_class_name << "::create_new_ic(uint8_t const cmd_key) {";

            c_fs << "\n";

            if (fixed_map) {

                c_fs << "\n    using CKE = " << cmd_class_name << "::Keys;";

                c_fs << "\n    auto const cmd_key_enum = static_cast<CKE>(cmd_key);\n";

                c_fs << "\n    switch(cmd_key_enum) {";
                // ***** key names for map ******
                cmd_custom_new_ic_map(cmds_json, c_fs, cmd_name, mc_cmd_map_size);
                c_fs << "\n    }";

                c_fs << "\n";

                c_fs << "\n    if (inner_commands.back() == nullptr) {"
                     << "\n        throw WarningException(\"" << cmd_class_name << "\","
                     << "\n                               \"create_new_ic\","
                     << "\n                               \"nullptr inner command: \" + std::to_string(cmd_key) );"
                     << "\n     }"
                     << "\n";

            } else {

                // ***** key names for normal multi command (all inner commands equal) ******
                cmd_custom_new_ic(cmds_json, c_fs, cmd_name, mc_cmd_map_size);
            }

            c_fs << "\n}";

        }

        // close files

        c_fs << "\n} // end namespace";
        c_fs.close();

    } // end for

    return 0;
};


