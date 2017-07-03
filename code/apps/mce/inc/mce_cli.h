// ** command line interface ** //
// Manzano Software //
#ifndef _MZN_MCE_CLI_H_
#define _MZN_MCE_CLI_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include "mzn_except.h"
#include "system_calls.h"
#include "user_interpreter.h"
#include "instruction_interpreter.h"
#include "json_sn.h"
#include "mcew_connection.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class MceCli {

private:
    TargetAddress ta_;

public:

   // ---------------------------------------------------------------------- //
    std::string config_file_path;
    std::string config_dir_path;
    McewConnection mcew_connection;

   // ---------------------------------------------------------------------- //
    explicit
    MceCli() : ta_{}, mcew_connection{} {

        try {

            config_dir_path = Utility::get_runtime_config_path();
            config_file_path = config_dir_path + "config.json";

        } catch (mzn::FatalException & e) {

            std::cout << std::endl << "No runtime configuration files found";
            auto const confirm = Utility::ask_yes("Create empty config file");
            if (confirm) {
                create_empty_config_file();
            } else throw e;
        }

        mcew_connection.config_dir_path = config_dir_path;
        mcew_connection.config_file_path = config_file_path;
    };


    ~MceCli() = default;

    //! mce cli starts here
    // ---------------------------------------------------------------------- //
    void user_input_loop();

    //! file management
    // ---------------------------------------------------------------------- //
    void create_empty_config_file();

    //! interactive local configuration editing
    // ---------------------------------------------------------------------- //
    void add_to_config(SeismicNetwork & sn,
                       std::string const & user_input,
                       TargetAddress & ta) const;

    void remove_from_config(SeismicNetwork & sn, TargetAddress & ta) const;
    void change_config(SeismicNetwork & sn, TargetAddress const & ta) const;
};

} // end namespace
#endif
