// ** command line interface ** //
// Manzano Software //
#ifndef _MZN_USER_INTERPRETER_H_
#define _MZN_USER_INTERPRETER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include "mzn_except.h"
#include "instruction_interpreter.h"
#include "system_calls.h"

namespace mzn {

class UserInterpreter {

public:

    explicit
    UserInterpreter() : ii{} {}
    ~UserInterpreter() = default;

    InstructionInterpreter ii;

    //! green manzano starts here
    //! either interactively
    void user_input_loop();
    //! or from a file
    void user_input_loop(std::string const & qrun_fname);

    //! user_input is one line in the console/file
    //! @calls instruction_interpreter
    void run_user_input(std::string const & user_input);

    //! match_* converts from a string to the correct type/value
    static
    Action match_action(std::string const & token);

    static
    Kind match_kind(std::string const & token);

    static
    Scope match_scope(std::string const & token, std::size_t & token_index);

    static
    TargetAddress match_target_address(std::string const & token,
                                       SeismicNetwork const & sn);
};

} // end namespace
#endif
