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

namespace mzn {

class MceCli {

public:

    explicit
    MceCli() : ta_{} {

        try {

            get_runtime_config_path();

        } catch(mzn::FatalException & e) {

            std::cout << std::endl << "No runtime configuration files found";
            std::cout << std::endl << "Create empty config file? (y/n): ";

            std::string response;
            getline(std::cin, response);

            if (response == "y") create_empty_config_file(); else throw e;
        }

        auto const home_path = get_environmental_variable("HOME");
        config_home_path = home_path + std::string("/.config/manzano");
    };

    std::string config_home_path;

    ~MceCli() = default;

    //! mce cli starts here
    void user_input_loop();

    void create_empty_config_file();

    void save_to_config_file(SeismicNetwork const & sn);

private:

    TargetAddress ta_;

};

} // end namespace
#endif
