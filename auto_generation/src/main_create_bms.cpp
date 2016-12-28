#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cctype>
#include <algorithm>

/* This file autogenerates:
   - one class for each bitmap in the bm json file

   For each bitmap class, it provides:
   -
*/
#include "json.h"
#include "ag_functions.h"

std::string make_classname_format(std::string bm_name) {

    //bm in camel case for class name
    bool capitalize_char = true;
    for (auto & c: bm_name) {
        if (capitalize_char) {
            c = std::toupper(c);
            capitalize_char = false;
        } else if (c == '_') {
            capitalize_char = true;
        }
    }

    // remove underscores
    bm_name.erase(
        std::remove(bm_name.begin(), bm_name.end(), '_'),
        bm_name.end());

    return bm_name;
}

int main() {

using Json = nlohmann::json;

std::cout << "bitmap class builder" << std::endl;

// *** BITMAP *** //

// open and parse the JSON bitmap format
// bm : bitmap
// bmf_fs : bitmap format file - file stream

std::ifstream bmf_fs;
bmf_fs.open("../bm_format.json");

if (not bmf_fs) {
    std::cerr << "ERROR: can't open cmd_field_bitmap_types format file" << std::endl;
    std::exit(EXIT_FAILURE);
}

std::stringstream bmf_ss;
bmf_ss << bmf_fs.rdbuf();

Json bms_json;
bms_json = Json::parse( bmf_ss.str() );

//! bm_h_fs : bm interface (.h) file stream
std::ofstream bm_h_fs;
//! bm_h_fs : bm implementation (.cpp) file stream
//std::ofstream bm_cpp_fs;

// cmd_field_bitmap_types.h file truncate
// if it doesn't exists, it creates it
bm_h_fs.open("../../code/cmd_field/inc/cmd_field_bitmap_types.h",
    std::ofstream::out | std::ofstream::trunc);

if (not bm_h_fs) {
    std::cerr << "ERROR: can't open cmd_field_bitmap_types.h file" << std::endl;
    std::exit(EXIT_FAILURE);
}

// file comments:
bm_h_fs << "// ** Bitmap Classes ** //\n";
bm_h_fs << "// Autogenerated in main_create_bms.cpp from bm_format.json ** //\n";
bm_h_fs << "// Manzano Software //\n";

// guards:
bm_h_fs << "\n#ifndef _MZN_CMD_FIELD_BITMAP_TYPES_H\n";
bm_h_fs << "#define _MZN_CMD_FIELD_BITMAP_TYPES_H\n";

// class includes:
bm_h_fs << "\n#include \"cmd_field_bitmap.h\"\n";
bm_h_fs << "\nnamespace mzn {";

// cmd_field_bitmap_types.cpp file truncate
//bm_cpp_fs.open("../../code/cmd_field/src/cmd_field_bitmap_types.cpp",
 //   std::ofstream::out | std::ofstream::trunc);
/*
if (!bm_cpp_fs) {
    std::cerr << "ERROR: can't open cmd_field_bitmap_types.cpp file" << std::endl;
    std::exit(EXIT_FAILURE);
}
*/

// include.h
//bm_cpp_fs << "\n#include \"cmd_field_bitmap_types.h\"\n\n";

//bm_cpp_fs << "\nnamespace mzn {";

// loop over the cmd_field_bitmap_types from the JSON
// adding to the file for interface/implementation files
// which get truncated everytime this program is run
// -------------------------------------------------------------------------- //
for (auto bm_itr = bms_json.begin(); bm_itr != bms_json.end(); ++bm_itr) {

    //! bm_name : bitmap name, i.e.: bm_cal_monitor_channel
    std::string bm_name = bm_itr.key();

    std::cout << std::endl << " *** processing bm: " << bm_name;
    std::cout << " ***" << std::endl;
    //! bm_classname : bm_name in camel calse, i.e.:BmCalMonitorChannel
    std::string bm_classname = make_classname_format(bm_name);

    //! mf_size : message field size. For CmdFieldBitmap, same as template
    //! parameter N, or number of bites in the underlaying std::bitset<N*8>
    int mf_size = bms_json[bm_name.c_str()]["mf_size"];

    //! bmfn : number of bitmap fields
    int bmfn = bms_json[bm_name.c_str()]["bm_fields"].size();

    bm_h_fs << "\n\n// -------------------------------------------------------------------------- //";

    //! bm_T : bitmap class type, created from class template CmdFieldBitmap<N>
    std::string bm_T = "CmdFieldBitmap<" + std::to_string(mf_size) + ">";

    bm_h_fs << "\nclass " << bm_classname << " : public " << bm_T << " {";

    bm_h_fs << "\n\nfriend std::ostream & operator<<(std::ostream & bm_os, ";
    bm_h_fs << bm_classname << " const & bm);";
    bm_h_fs << "\n";

    bm_h_fs << "\npublic:";
    // constructor
    bm_h_fs << "\n    explicit " << bm_classname << "() : " << bm_T << "{} {};";

    //! bmf : bitmap field, data field found in the bitmap
    /*!
      Types of bitmap fields and components:

            common component:
            - bmf_name:
              the name of the data field. i.e.: pll_status

         ** Binary value: one bit in the bitmap represents a boolean value.
            i.e.: thread_blocked

            components:
            - bmf_bit: the location of the bit corresponding to the bmf

         ** Code value: Contiguous bits in the bitset represent a code
            that maps to one option on a list.
            i.e.:
            Pll Status:
            0b00 = PLL not enabled. (bmf_code = 0)
            0b01 = Hold             (bmf_code = 1)
            0b10 = Tracking         (bmf_code = 2)
            0b11 = Locked           (bmf_code = 3)

            components:
            - bmf_code_name:
              the name of one of the possible options in the list.
              i.e: pll_not_enabled

            - bmf_code:
              the value (as unsigned int) of the evaluated range of bits
              corresponding to the associated bmf_name.
              i.e.: 0 for pll_not_enabled

            - bmf_codes:
              the container of codes in the list.

            - bmf_code_full_range:
              boolean, if evaluating the entire bitset data is the same
              as evaluating the bmf_code bits.
              i.e.:
              with xx refering to the location of the range of 2 relevant bits
              0b000000xx -> code full range, 0b11 == 0b00000011
              0b0000xx00 -> code not full range, 0b11 != ob00001100
              0b001010xx -> code not full range, 0b11 != ob00101011

            - bmf_code_begin:
              the location of range of bits where the code is located.
              i.e.:
              with xx refering to the location of the range of 2 relevant bits
              0b000000xx -> bmf_code_begin = 0
              0b0000xx00 -> bmf_code_begin = 3
              0b0xx01001 -> bmf_code_begin = 6

            - bmf_code_mask:
              A mask with the same number of bits set as
              the number of bits in the code.
              i.e.:
              with xx refering to the location of the range of 2 relevant bits
              0b000000xx -> bmf_code_mask = 0b11 = 3
              0b0000xxxx -> bmf_code_mask = 0b1111 = 15
              0b0xxxx001 -> bmf_code_mask = 0b1111 = 15

         ** Raw value: range of bits in the bitmap represents a unsigned int
            value.
            i.e.: global_event_value

            components:
            same to bmf_code_begin / bmf_code_mask

            - bmf_val_begin:
              the location of the bits where the raw value is located.

            - bmf_val_mask: the location of the bit corresponding to the bmf
              A mask with the same number of bits set as
              the number of bits in the raw value.

    */

    //! ------------- bmf_code enum ------------- //
    /*!
      for each bitmap field with a code. An enum is created, where:
        an enum value will be (with namespace):
        bm_class_name::bmf_class_name::bmf_code_name

        the value can be compared with the enum
            i.e.:
            BmPllState::PllStatus::pll_not_enabled == bm_pll_state.pll_status()

        or set with the enum
            i.e.:
            bm_pll_state.pll_status( BmPllState::PllStatus::pll_not_enabled );
    */

    // loop over bitmap fields, make enums
    for (int bmfi = 0; bmfi < bmfn; bmfi++) {

        auto bmf_json = bms_json[bm_name.c_str()]["bm_fields"][bmfi];

        std::string bmf_name = bmf_json["bmf_name"];

        std::string bmf_class_name = make_classname_format(bmf_name);

        // do enums for bmf codes only
        if ( json_has_key(bmf_json, "bmf_codes") ) {

            std::cout << "\nEnums for: " << bmf_name << std::endl;

            //! bmf_cn : number of bitmap field codes
            int bmf_cn = bmf_json["bmf_codes"].size();

            bm_h_fs << "\n\n    ";
            bm_h_fs << "enum class " << bmf_class_name << " {";

            for (int bmf_ci = 0; bmf_ci < bmf_cn; bmf_ci++) {

                // code name
                std::string bmf_code_name =
                    bmf_json["bmf_codes"][bmf_ci]["bmf_code_name"];

                int bmf_code =
                    bmf_json["bmf_codes"][bmf_ci]["bmf_code"];

                bm_h_fs << "\n        ";
                bm_h_fs << bmf_code_name << " = " << bmf_code << ",";
            }

            bm_h_fs << "\n    };\n";
        }
    } // end for, enums for codes

    //! ------------- getter ---------------- //
    /*!
      for each bitmap field. A getter is created, where:
        - bmf bits return boolean,
            i.e.:
            const bool negative_step() const;

        - bmf codes return the code value,
            i.e.:
            const unsigned long pll_status() const;
            (see enum for codes)

        - bmf raw vals return value,
            i.e.:
            const unsigned long global_event_val() const;
    */
    for (int bmfi = 0; bmfi < bmfn; bmfi++) {

        auto bmf_json = bms_json[bm_name.c_str()]["bm_fields"][bmfi];

        std::string bmf_name = bmf_json ["bmf_name"];

        std::cout << "\nGetters for: " << bmf_name << std::endl;

        auto has_bmf_bit = json_has_key(bmf_json, "bmf_bit");

        if ( has_bmf_bit ) {

            // ***  boolean/binary value in bitset ***

            int bmf_bit = bmf_json["bmf_bit"];

            bm_h_fs << "\n    ";
            bm_h_fs << "bool const " << bmf_name << "() const {";
            bm_h_fs << "return this -> data_.test(" << bmf_bit << ");}";


        } else if ( json_has_key(bmf_json, "bmf_codes") ) {

            // *** code in bitset ***
            // bmf_code variables as defined above

            // explanation of variables above
            // full range?
            bool bmf_code_full_range =
                bmf_json["bmf_code_full_range"];


            bm_h_fs << "\n    ";
            bm_h_fs << "unsigned long const " << bmf_name << "() const {";

            std::string bmf_class_name = make_classname_format(bmf_name);

            if(bmf_code_full_range) {
                // the entire bitset can be evaluated
                bm_h_fs << "return " << bm_T << "::raw_value();}";

            } else {

                // the code is located in a range of the bitset
                int bmf_code_begin =
                    bmf_json["bmf_code_begin"];

                std::string bmf_code_mask =
                    bmf_json["bmf_code_mask"];


                bm_h_fs << "return " << bm_T
                        << "::raw_value_from_range(" << bmf_code_begin
                        << ", " << bmf_code_mask << ");}";

            } // end of getter for code

        // *** raw value in bitset ***
        } else if ( json_has_key(bmf_json, "bmf_val_begin") ) {

            int bmf_val_begin = bmf_json["bmf_val_begin"];
            std::string bmf_val_mask = bmf_json["bmf_val_mask"];

            bm_h_fs << "\n    ";
            bm_h_fs << "const unsigned long " << bmf_name << "() const {";
            bm_h_fs << "return " << bm_T << "::raw_value_from_range(" << bmf_val_begin << ", ";
            bm_h_fs << bmf_val_mask << ");}";

        } else {
            std::cerr << "\n\n ------- ERROR ------------ \n Unexpected bitmap type";
            std::exit(EXIT_FAILURE);
        }
    } // end for, getter

    //! ------------- setter ---------------- //
    /*!
      for each bitmap field. A setter is created, where:
        - bmf bits set boolean values,
            i.e.:
            void negative_step(bool const b);

        - bmf codes set the code value,
            i.e.:
            void pll_status(unsigned int const c);
            (see enum for codes)

        - bmf raw vals set the value,
            i.e.:
            void global_event_val(unsigned int const v);
    */

    // information originating in the digitizer (q)
    // does not need a setter
    std::string bm_originator =
        bms_json[bm_name.c_str()]["bm_originator"];

    if (bm_originator != "q") {
        // loop over bitmap fields
        for (int bmfi = 0; bmfi < bmfn; bmfi++) {

            auto bmf_json = bms_json[bm_name.c_str()]["bm_fields"][bmfi];

            std::string bmf_name = bmf_json["bmf_name"];

            std::string bmf_class_name = make_classname_format(bmf_name);

            if (json_has_key(bmf_json, "bmf_bit")) {
                // binary value

                int bmf_bit =
                    bmf_json["bmf_bit"];

                bm_h_fs << "\n    ";
                bm_h_fs << "void " << bmf_name << "(const bool b) {";
                bm_h_fs << "this -> data_.set(" << bmf_bit << ", b);}";

            } else if (json_has_key(bmf_json, "bmf_codes")) {

                // full range?
                bool bmf_code_full_range =
                    bmf_json["bmf_code_full_range"];

                if (not bmf_code_full_range) {

                    // the code is located in a range of the bitset
                    int bmf_code_begin =
                        bmf_json["bmf_code_begin"];

                    std::string bmf_code_mask =
                        bmf_json["bmf_code_mask"];

                    bm_h_fs << "\n    ";
                    bm_h_fs << "void " << bmf_name << "(" << bmf_class_name << " const c) {";
                    bm_h_fs << bm_T << "::set_raw_value_in_range(" << bmf_code_begin << ", "
                                                               << bmf_code_mask
                                                               << ", static_cast<unsigned long>(c) );}";
                } else {
                    // the whole bitset is a code

                    bm_h_fs << "\n    ";
                    bm_h_fs << "void " << bmf_name << "(" << bmf_class_name << " const c) {";
                    bm_h_fs << "this -> data_ = std::bitset<" << (mf_size * 8)
                            << ">( static_cast<unsigned long>(c) );}";
                }
            }

        } // end for, setter bmf loop
    } // end if,  setter

    bm_h_fs << "\n};\n"; // end of class declaration

    // ------------------------------------------------------------- //
    //                      OPERATOR<< OVERLOAD                      //
    // ------------------------------------------------------------- //


    bm_h_fs << "\ninline std::ostream & operator<<(std::ostream & bm_os, "<< bm_classname << " const & bm) {";

    // new line in bm stream
    bm_h_fs << "\n    bm_os << \"\\n\";";

    // loop over bitmap fields and stream data
    for (int bmfi = 0; bmfi < bmfn; bmfi++) {

        auto bmf_json = bms_json[bm_name.c_str()]["bm_fields"][bmfi];

        std::string bmf_name = bmf_json["bmf_name"];

        if (json_has_key(bmf_json, "bmf_bit")) {

            // *** binary value ****
            bm_h_fs << "\n    bm_os << \"\\n   [\" << bm.bool_indicator( ";
            bm_h_fs << "bm." << bmf_name << "() ) << \"] \" <<\n        ";
            bm_h_fs << "\"" << bmf_name << "\";";

        } else if (json_has_key(bmf_json, "bmf_codes")) {
            // *** code in bitset ***
            // in this case, in addition to the value of the field
            // a description of the code is wanted

            // code name and code
            std::string bmf_name =
                bmf_json["bmf_name"];

            bm_h_fs << "\n // just in case there is more than one code in a bmf";
            bm_h_fs << "\n // as in BmAuxStatusWebFlags";
            bm_h_fs << "\n{";
            // value first using getter
            bm_h_fs << "\n    unsigned int c = " << "bm." << bmf_name << "();";
            bm_h_fs << "\n    bm_os << "  << "\"\\n   ";
            bm_h_fs  << bmf_name << " : \"";
            bm_h_fs << " << c << \" : \";";

            // description:
            bm_h_fs << "\n    switch(c) {";

            int bmf_cn = bmf_json["bmf_codes"].size();

            // loop over codes of this bitmap field
            for (int bmf_ci = 0; bmf_ci < bmf_cn; bmf_ci++) {

                int bmf_code =
                    bmf_json["bmf_codes"][bmf_ci]["bmf_code"];

                std::string bmf_code_name =
                    bmf_json["bmf_codes"][bmf_ci]["bmf_code_name"];

                // for the code with the code name
                bm_h_fs << "\n\n    case " << bmf_code << " : {";
                bm_h_fs << "\n         bm_os << \"" << bmf_code_name << "\";";
                bm_h_fs << "\n         break;}";
            } // end for, bmf codes loop for stream

            bm_h_fs << "\n    }";
            bm_h_fs << "\n}";

        } else if (json_has_key(bmf_json, "bmf_val_begin")) {

            // *** raw value in bitset ***
            // use the getter for the value

            bm_h_fs << "\n    bm_os << ";
            bm_h_fs << "\"\\n   " << bmf_name << " : \"";
            bm_h_fs << " << " << "bm." << bmf_name << "();";

        } // end if, bmf types
    } // end for, operator<< bmf loop

    bm_h_fs << "\n    bm_os << \"\\n\";";
    bm_h_fs << "\n    return bm_os;";
    bm_h_fs << "\n}";

} // end of bm iterator

//bm_cpp_fs << "\n} // end of namespace";
// close file
//bm_cpp_fs.close();

// guards end if
bm_h_fs << "\n} // end of namespace";
bm_h_fs << "\n" << "#endif // _MZN_CMD_FIELD_BITMAP_TYPES_H\n";

bm_h_fs.close();

return 0; // main
};


