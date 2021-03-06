#include "user_interpreter.h"
#include <fstream>
#include <string>
#include <iterator>

namespace mzn {

// -------------------------------------------------------------------------- //
Action UserInterpreter::match_action(std::string const & token) {

    if (token == "show")    return Action::show;
    if (token == "edit")    return Action::edit;
    if (token == "get")     return Action::get;
    if (token == "set")     return Action::set;
    if (token == "plan")    return Action::plan;
    if (token == "auto")    return Action::auto_;
    if (token == "stop")    return Action::stop;
    if (token == "start")   return Action::start;

    // no match found
    std::stringstream error_msg;
    error_msg << "mismatch in action token \'" << token << "\' \n"
              << "\nactions: "
              << "\nshow edit get set plan auto stop start";

    throw WarningException("UserInterpreter",
                            "match_action",
                            error_msg.str() );
}

// -------------------------------------------------------------------------- //
Kind UserInterpreter::match_kind(std::string const & token) {

    if (token == "target")  return Kind::target;
    if (token == "config")  return Kind::config;
    if (token == "status")  return Kind::status;
    if (token == "help")    return Kind::help;
    if (token == "poll")    return Kind::poll;
    if (token == "global")  return Kind::global;
    if (token == "ping")    return Kind::ping;
    if (token == "stat")    return Kind::stat;
    if (token == "qview")   return Kind::qview;
    if (token == "ctrl")    return Kind::ctrl;
    if (token == "reg")     return Kind::reg;
    if (token == "dereg")   return Kind::dereg;
    if (token == "center")  return Kind::center;
    if (token == "cal")     return Kind::cal;
    if (token == "pulse")   return Kind::pulse;
    if (token == "uptime")  return Kind::uptime;
    if (token == "plan")    return Kind::plan;
    if (token == "link")    return Kind::link;
    if (token == "token")   return Kind::token;
    if (token == "wait")    return Kind::wait;
    if (token == "dev")     return Kind::dev;
    if (token == "mod")     return Kind::mod;
    if (token == "output")  return Kind::output;

    // no kind found for that token
    //TODO: use InstructionMap
    std::stringstream error_msg;
    error_msg << "mismatch in kind token \'" << token << "\' \n"
              << "\nkinds: "
              << "\ntarget config status help poll global ping stat qview dev"
              << "\nctrl reg dereg center cal pulse plan output";

    throw WarningException("UserInterpreter",
                           "match_kind",
                           error_msg.str() );
}

// -------------------------------------------------------------------------- //
Scope UserInterpreter::match_scope(std::string const & token,
                                   std::size_t & token_index) {

    Scope scope;

    //! match by scope short name
    // there should be only ONE of each
    // something like q[0]q[1] would be an error

    if (token.find("q", token_index) == token_index) {
        scope = Scope::digitizer;
        token_index += 1;
    } else if (token.find("st", token_index) == token_index) {
        scope = Scope::station;
        token_index += 2;
    } else if (token.find("s", token_index) == token_index) {
        scope = Scope::sensor;
        token_index += 1;
    } else if (token.find("dp", token_index) == token_index) {
        scope = Scope::data_processor;
        token_index += 2;
    }

    else {

        std::stringstream error_msg;

        error_msg << "mismatch in token \'" << token << "\' \n";

        if ( not token.empty() ) {
            error_msg << "at:" << Utility::underline_error(token, token_index);
        }

        error_msg << "\noptions: "
                  << "\nq st s dp";

        throw WarningException("UserInterpreter",
                               "match_scope",
                               error_msg.str() );
    }

    return scope;
}

// -------------------------------------------------------------------------- //
TargetAddress
UserInterpreter::match_target_address(std::string const & token,
                                      SeismicNetwork const & sn) {

    // format SCOPE NUMBER (SCOPE NUMBER)*optionally
    // examples of the token string: "sn", "st0", "st1q0", "st0q0s1"

    // apply to all seismic network
    if (token == "sn") return TargetAddress{};

    TargetAddress ta;

    // gets changed on the match_* with 2 arguments
    std::size_t token_index = 0;

    // while loop in case the address has more than one target in the ta
    while ( token_index < token.size() ) {

        Target target;

        if (token[token_index] == '[' and token_index == 0) {

            target.scope = Scope::station;

            auto const end_bracket_index = token.find(']');
            if (end_bracket_index == std::string::npos) {
                throw WarningException("UserInterpreter",
                                       "match_target_address",
                                       "bracket '[' not matched with a ']'");
            }
            // substring inside the brackets
            auto station_name = token.substr(1, end_bracket_index - 1);
            // capitalize station name
            for (auto & c: station_name) c = std::toupper(c);
            target.index = sn.station_index(station_name);
            token_index += end_bracket_index + 1;

        } else {
            target.scope = match_scope(token, token_index);
            target.index = Utility::match_positive_number(token, token_index);
        }

        ta.add_target(target);
    }

    return ta;
}

// requires user_input not empty, checked before calling this function
// -------------------------------------------------------------------------- //
void UserInterpreter::run_user_input(std::string const & user_input) {

    // container with all the token strings, checks number of tokens
    auto input_tokens = Utility::get_tokens(user_input, ' ');

    // ---------------------------------------------------------------------- //
    if ( input_tokens.empty() ) throw std::logic_error("run_user_input");

    // shortcut for edit target target_address
    // ---------------------------------------------------------------------- //
    if (input_tokens.size() == 1) {
        // see comments below for respective functions
        auto ta = match_target_address(input_tokens[0], ii.cm.sn);
        ii.merge_and_check_target_address(ta);

        auto constexpr action = Action::edit;
        auto constexpr kind = Kind::target;
        std::string const option = "";
        auto const ui = UserInstruction(action, kind, option);
        ii.run_instruction(ui, ta);
        return;
    }

    // at least 2 elements on input_tokens, must be action kind
    // ---------------------------------------------------------------------- //
    // order in which the tokens will be provided, for example:
    // get ping q0 = action kind target_address
    auto constexpr action_index = 0;
    auto constexpr kind_index = 1;

    // option ':'
    // ---------------------------------------------------------------------- //
    std::string option;
    // an option can be specified by adding the mark ':', for example stat:boom
    // where "stat" is the Kind and "boom" is the option
    auto option_mark_location = input_tokens[kind_index].find(':');

    // the option mark ':' is found and is not the last char of the string
    if (option_mark_location != std::string::npos and
        input_tokens[kind_index].back() != ':') {

        // get and then erase the option from the kind token
        // get without the option mark: in "stat:boom", option = "boom"
        // erase including the option mark: from "stat:boom" to "stat"
        option = std::string(input_tokens[kind_index], option_mark_location + 1);
        input_tokens[kind_index].erase(option_mark_location);
    }

    // ---------------------------------------------------------------------- //
    auto const action = match_action(input_tokens[action_index]);
    auto const kind = match_kind(input_tokens[kind_index]);
    auto const ui = UserInstruction(action, kind, option);

    // dash '-'
    // ---------------------------------------------------------------------- //
    // a dash instruction can be specified by dash and letter '-x',
    // for example get ping -i (preview input only)
    std::string dash;
    while (input_tokens.back()[0] == '-' and input_tokens.back().size() == 2) {
        dash += input_tokens.back()[1];
        input_tokens.pop_back();
    }
    // std::cout << std::endl << "dash: [" << dash << "]";

    // ta 'st?q?s?'
    // ---------------------------------------------------------------------- //
    if (input_tokens.size() == 2) {

        // use the current target address
        // no merging/checking of target_address needed
        ii.check_instruction_map(ui);
        ii.run_instruction(ui);

    } else if (input_tokens.size() == 3) {

        auto constexpr target_address_index = 2;

        // matches to whatever the user wrote, it can create an erroneous ta
        // like "st0s0" (instead of st0q0s0 or q0s0 or s0), gets checked next
        // it can also create an incomplete ta, like "s0" which is ok if
        // it can be merged on the next steps
        auto ta = match_target_address(input_tokens[target_address_index],
                                       ii.cm.sn);

        // also does basic checks for the target address
        // do the different targets in ta, actually exists in the sn?
        ii.merge_and_check_target_address(ta);

        // is this intructin valid? good: get ping, bad: set ping
        ii.check_instruction_map(ui, ta);
        ii.run_instruction(ui, ta);

    } else {

        // input_tokens.size() > 3
        std::stringstream error_msg;
        error_msg << "too many arguments";

        error_msg << "\n\narguments received       : ";
        for (auto const & token : input_tokens) error_msg << token << " ";

        error_msg << "\narguments not understood : ";
        for (int i = 3; i < input_tokens.size() ; i++) {
            error_msg << input_tokens[i] << " ";
        }

        error_msg << "\n\n";
        throw WarningException("UserInterpreter",
                               "run_user_input",
                               error_msg.str() );
    }
}

// -------------------------------------------------------------------------- //
void UserInterpreter::user_input_loop(std::string const & qrun_fname) {

    auto const runtime_config_path = Utility::get_runtime_config_path();
    std::string const qrun_path = runtime_config_path + "/" + qrun_fname;

    std::ifstream qrun_fs(qrun_path);
    //qrun_fs.open(qrun_path);

    if (not qrun_fs) {
        throw FatalException("UserInterpreter",
                             "user_input_loop",
                             std::string("can't open script file") + qrun_path);
    }

    std::string user_input;

    while ( std::getline(qrun_fs, user_input) ) {

        try {

            ii.show_prompt();
            std::cout << user_input;

            if (user_input == "") continue;

            if (user_input == "..") {
                ii.current_ta_remove_one_target();
                continue;
            }

            //! quit program normally
            if (user_input == "quit") {

                UserInstruction const ui(Action::quit, Kind::mzn);
                ii.run_instruction(ui);
                std::cout << std::endl << "\nbye" << std::endl;
                break;
            }

            run_user_input(user_input);

        } catch (Exception const & e) {

            std::cerr << std::endl
                      << "caught error @ UserInterpreter::user_input_loop(qrun)"
                      << std::endl << e.what();
            break;
        }
    }
}

// -------------------------------------------------------------------------- //
void UserInterpreter::user_input_loop() {
    // prompts the user for their command
    // interactive
    std::string user_input;

    while (true) {

        ii.show_prompt();

        std::getline(std::cin, user_input);
        std::cout << std::flush << "### " << Time::sys_time_of_day() << " ###";

        try {

            if (user_input == "") continue;

            if (user_input == "..") {
                ii.current_ta_remove_one_target();
                continue;
            }

            //! quit program normally
            if (user_input == "quit") {

                UserInstruction const ui(Action::quit, Kind::mzn);
                ii.run_instruction(ui);
                std::cout << std::endl << "\nbye" << std::endl;
                break;
            }

            run_user_input(user_input);

        } catch (FatalException const & e) {
            std::cerr << std::endl
                      << "caught fatal error @UserInterpreter::user_input_loop()"
                      << std::endl << e.what();
            break;

        } catch (Exception const & e) {
            std::cerr << std::endl
                      << "caught error @UserInterpreter::user_input_loop()"
                      << std::endl << e.what();
        }
    }
}

} // <- mzn
