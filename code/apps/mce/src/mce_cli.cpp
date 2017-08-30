// ** command line interface ** //
// Manzano Software //
#include <cstdlib>
#include "mce_cli.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void MceCli::user_input_loop() {

    // prompts the user for their command
    // interactive
    std::string user_input;

    while (true) {

        // rebuild TODO change sn contructor, total rebuild not needed
        // ----------------------------------------------------------------- //
        InstructionInterpreter ii(ta_);
        auto & sn = ii.cm.sn;

        // check ta_ again, this time without catching
        // ----------------------------------------------------------------- //
        ii.check_ta_in_sn(ta_);


        // show prompt
        // ------------------------------------------------------------------ //
        ii.cm.stream_output.show<Kind::target>(ta_);
        std::cout << "\n---------------------------------------------";
        ii.cm.stream_output.show_prompt(ta_);

        // ------------------------------------------------------------------ //
        std::getline(std::cin, user_input);

        try {

            //! user hit enter
            // -------------------------------------------------------------- //
            if (user_input == "") continue;

            //! ta to parent
            // -------------------------------------------------------------- //
            if (user_input == "..") {
                ta_.remove_one_target();
                continue;
            }

            // -------------------------------------------------------------- //
            if (user_input == "rm") {
                // changes ta_ to parent
                remove_from_config(sn, ta_);
                continue;
            }

            // -------------------------------------------------------------- //
            if (user_input == "ed") {
                // same ta_
                change_config(sn, ta_);
                continue;
            }

            //! stream raw json format
            // -------------------------------------------------------------- //
            if (user_input == "raw") {
                auto const json = Utility::json_from_ta(sn, ta_);
                std::cout << json.dump(4) << std::endl;
                continue;
            }

            // -------------------------------------------------------------- //
            if (user_input[0] == '+') {
                // changes ta_ to newly added
                add_to_config(sn, user_input, ta_);
                continue;
            }

            // -------------------------------------------------------------- //
            if (user_input == "csv") {
                csv_to_config(sn);
                continue;
            }

            // -------------------------------------------------------------- //
            if (user_input == "@") {
                mcew_connection.update_all(sn);
                continue;
            }

            // -------------------------------------------------------------- //
            auto pack_station_args = [&user_input](char const c) {

                if (user_input.size() < 3 or user_input[1] != ' ') {

                    std::stringstream ss;
                    ss << "wrong format for station names, example: "
                       << c << " ST1 ST2";

                    throw WarningException( "MceCli",
                                            "user_input_loop",
                                            ss.str() );
                }

                auto const args = user_input.substr(2);
                auto station_names = Utility::get_tokens(args, ' ');
                Utility::capitalize_tokens(station_names);
                return station_names;
            };

            // -------------------------------------------------------------- //
            auto const & first_char = user_input[0];

            //! replace current stations with the stations requested from mcew
            // -------------------------------------------------------------- //
            if (first_char == '!') {
                auto const station_names = pack_station_args(first_char);
                mcew_connection.use(sn, station_names);
                // the current ta might not be in the configuration
                // default to top of the sn consistently
                ta_ = TargetAddress{};
                continue;
            }

            //! update current stations with the stations requested from mcew
            // -------------------------------------------------------------- //
            if (first_char == '@') {
                auto const station_names = pack_station_args(first_char);
                mcew_connection.update(sn, station_names);
                continue;
            }

            //! add stations from mcew
            // -------------------------------------------------------------- //
            if (first_char == '<') {
                auto const station_names = pack_station_args(first_char);
                mcew_connection.get(sn, station_names);
                continue;
            }

            //! send stations to mcew
            // -------------------------------------------------------------- //
            if (first_char == '>') {
                auto const station_names = pack_station_args(first_char);
                mcew_connection.send(sn, station_names);
                continue;
            }

            // -------------------------------------------------------------- //
            if (user_input == "help") {
                std::cout << "\n"
                          << "\n   usage:"
                          << "\n   ===================="
                          << "\nrm      : remove current target"
                          << "\ned      : edit current target"
                          << "\nraw     : view raw json format"
                          << "\n+st     : add station to sn"
                          << "\n+q      : add digitizer to st"
                          << "\n+s      : add sensor to q"
                          << "\n+dp     : add data_processor to st"
                          << "\n..      : select parent target"
                          << "\nsn      : select seismic network"
                          << "\nst#     : select st[#]"
                          << "\n[ST1]   : select station ST1"
                          << "\nst#q#   : select st[#]q[#]"
                          << "\nst#q#s# : select st[#]q[#]s[#]"
                          << "\nst#dp#  : select st[#]dp[#]"
                          << "\n"
                          << "\n   mce web service:"
                          << "\n   ===================="
                          << "\n! ST1 ST2 : replace config with these stations"
                          << "\n@ ST1 ST2 : update these stations"
                          << "\n< ST1 ST2 : add these stations"
                          << "\n> ST1 ST2 : send these stations"
                          << "\n@         : update all current stations"
                          << "\n"
                          << "\n   relative addressing:"
                          << "\n   ===================="
                          << "\nst[#]     » q#  : select st[#]q[#]"
                          << "\nst[#]q[#] » s#  : select st[#]q[#]s[#]"
                          << "\nst[#]     » dp# : select st[#]dp[#]"
                          << "\n"
                          << "\n   confirmation:"
                          << "\n   ============="
                          << "\n   press enter to keep original value: "
                          << "\n        input (A or B)[\"B\"]: "
                          << "\n        Has E300? (y/n) [true]: "
                          << "\n   or change original value: "
                          << "\n        input (A or B)[\"B\"]: A"
                          << "\n        Has E300? (y/n) [true]: n";
                continue;
            }

            // -------------------------------------------------------------- //
            if (user_input == "quit") break;

            //! only thing left is a target address
            // -------------------------------------------------------------- //
            auto ta = UserInterpreter::match_target_address(user_input,
                                                            ii.cm.sn);
            ta.add_targets_from_ta(ta_);
            ii.check_ta_in_sn(ta);
            ta_ = ta;

        } catch (FatalException const & e) {

            std::cerr << std::endl
                      << "caught fatal error @Mce::user_input_loop()"
                      << std::endl << e.what();
            break;

        } catch (Exception const & e) {

            std::cerr << std::endl
                      << "caught error @Mce::user_input_loop()"
                      << std::endl << e.what();
        }
   }
    std::cout << std::endl << "\nbye" << std::endl;
}


// -------------------------------------------------------------------------- //
void MceCli::create_empty_config_file() {

    // use home as default
    auto const home_path = Utility::get_environmental_variable("HOME");
    auto const config_home_path = home_path + std::string("/.config/manzano");

    std::cout << std::endl << "config path: " << config_home_path;

    std::ofstream config_fs;

    config_fs.open(config_home_path + "/config.json",
                   std::ofstream::out | std::ofstream::trunc);

    config_fs << "{ \"station\": [] }";
}

// for interactive filling of target information
// -------------------------------------------------------------------------- //
void MceCli::add_to_config(SeismicNetwork & sn,
                           std::string const & user_input,
                           TargetAddress & ta) const {

    std::size_t scope_pos = 1;

    auto child_scope = UserInterpreter::match_scope(user_input, scope_pos);
    std::cout <<  "\nAdding a " << child_scope << " to " << ta << "\n";
    auto child_json = Utility::json_add_child_from_ta(sn, ta, child_scope);
    std::cout << std::endl << child_json.dump(4);

    auto const confirm = Utility::ask_yes("Looks good");
    if (not confirm) {std::cout << "OK, no changes"; return;}

    TargetAddress child_ta;

    // it's on
    // --------------------------------------------------------------------- //
    switch (child_scope) {

        case Scope::data_processor: {
            auto & st = sn.st_ref(ta);
            st.dp.push_back( Utility::dp_from_json(child_json) );
            auto const child_index = st.dp.size() - 1;
            auto const child_target = Target(child_scope, child_index);
            child_ta.st_child = child_target;
            break;
        }

        case Scope::sensor: {
            auto & q = sn.q_ref(ta);
            q.s.push_back( Utility::s_from_json(child_json) );
            auto const child_index = q.s.size() - 1;
            auto const child_target = Target(child_scope, child_index);
            child_ta.q_child = child_target;
            break;
        }

        case Scope::digitizer: {
            auto & st = sn.st_ref(ta);
            st.q.push_back( Utility::q_from_json(child_json) );
            auto const child_index = st.q.size() - 1;
            auto const child_target = Target(child_scope, child_index);
            child_ta.st_child = child_target;
            break;
        }

        case Scope::station: {
            sn.st.push_back( Utility::st_from_json(child_json) );
            auto const child_index = sn.st.size() - 1;
            auto const child_target = Target(child_scope, child_index);
            child_ta.sn_child = child_target;
            break;
        }

        default: throw std::logic_error{"@MceCli::add_to_config"};
    }

    Utility::save_to_config_file(sn, config_file_path);

    // child_ta is incomplete, lets add the parents
    child_ta.add_targets_from_ta(ta);
    // this should make it easier to modify this newly created object
    ta = child_ta;
}

// -------------------------------------------------------------------------- //
void MceCli::remove_from_config(SeismicNetwork & sn,
                                TargetAddress & ta) const {

    // confirm first
    auto const target = ta.target();
    std::cout <<  "\nRemoving " << target << " from " << ta << "\n";
    auto json = Utility::json_from_ta(sn, ta);
    std::cout << std::endl << json.dump(4);

    auto const confirm = Utility::ask_yes("Remove");
    if (not confirm) {std::cout << "OK, no changes"; return;}

    // since digitizers (and then stations) can't have copy/move assignment
    // due to const members by design, std::vector<T>.erase(..) can't be used.
    // However, move constructors are provided.
    // --------------------------------------------------------------------- //
    auto remove_without_assignment = [](auto & v, auto const pos) {

        // reset v
        auto temp_v = std::move(v);
        v.clear();

        // refill v without the element
        for (int i = 0; i < temp_v.size(); i++) {
            // use move constructor which is defined
            if (i != pos) v.push_back( std::move(temp_v[i]) );
        }
    };

    // it's on
    // --------------------------------------------------------------------- //
    switch (target.scope) {

        case Scope::data_processor: { auto & st = sn.st_ref(ta);
            st.dp.erase(st.dp.begin() + target.index);
            break;
        }

        case Scope::sensor: {
            auto & q = sn.q_ref(ta);
            q.s.erase(q.s.begin() + target.index);
            break;
        }

        case Scope::digitizer: {
            auto & st = sn.st_ref(ta);
            remove_without_assignment(st.q, target.index);
            break;
        }

        case Scope::station: {
            remove_without_assignment(sn.st, target.index);
            break;
        }

        case Scope::seismic_network: {
            sn.st.clear();
            break;
        }

        default: throw std::logic_error{"@MceCli::add_to_config"};
    }

    Utility::save_to_config_file(sn, config_file_path);

    // this target no longer exist, so ta is set to its parent
    ta.remove_one_target();
}

// -------------------------------------------------------------------------- //
void MceCli::change_config(SeismicNetwork & sn,
                           TargetAddress const & ta) const {

    // confirm first
    auto const target = ta.target();
    std::cout << "\nChanging " << target << " from " << ta << "\n";
    auto original_json = Utility::json_from_ta(sn, ta);

    std::cout << "\n == JSON original ==";
    std::cout << std::endl << original_json.dump(4);

    // since digitizers (and then stations) can't have copy/move assignment
    // due to const members by design, std::vector<T>.at(..) can't be used.
    // However, move constructors are provided.
    // --------------------------------------------------------------------- //
    auto replace_without_assignment = [](auto & v,
                                         auto const pos,
                                         auto & rhs) {
        // reset v
        auto temp_v = std::move(v);
        v.clear();

        // refill v with a different element rhs at pos
        for (int i = 0; i < temp_v.size(); i++) {
            // use move constructor which is defined
            if (i == pos) v.push_back( std::move(rhs) );
            else v.push_back( std::move(temp_v[i]) );
        }
    };

    // --------------------------------------------------------------------- //
    auto cancel_change = [](auto & json) {
        std::cout << "\n == JSON modified ==";
        std::cout << std::endl << json.dump(4);
        auto const confirm = Utility::ask_yes("Change");
        if (not confirm) std::cout << "OK, no changes";
        return not confirm;
    };

    // each one confirms
    // --------------------------------------------------------------------- //
    switch (target.scope) {

        case Scope::data_processor: {
            auto & st = sn.st_ref(ta);
            auto json = Utility::json_change_dp(original_json);
            if ( cancel_change(json) ) return;
            auto dp = Utility::dp_from_json(json);
            replace_without_assignment(st.dp, target.index, dp);
            break;
        }

        case Scope::sensor: {
            auto & q = sn.q_ref(ta);
            auto json = Utility::json_change_s(original_json);
            if ( cancel_change(json) ) return;
            auto s = Utility::s_from_json(json);
            replace_without_assignment(q.s, target.index, s);
            break;
        }

        case Scope::digitizer: {
            auto & st = sn.st_ref(ta);
            auto json = Utility::json_change_q(original_json);
            if ( cancel_change(json) ) return;
            auto q = Utility::q_from_json(json);
            replace_without_assignment(st.q, target.index, q);
            break;
        }

        case Scope::station: {
            auto json = Utility::json_change_st(original_json);
            if ( cancel_change(json) ) return;
            auto st = Utility::st_from_json(json);
            replace_without_assignment(sn.st, target.index, st);
            break;
        }

        case Scope::seismic_network: {
            throw WarningException("MceCli",
                                   "change_config",
                                   "sn not a change target");
        }

        default: throw std::logic_error{"@MceCli::add_to_config"};
    }

    Utility::save_to_config_file(sn, config_file_path);
}

// for filling from csv file
// -------------------------------------------------------------------------- //
void MceCli::csv_to_config(SeismicNetwork & sn) const {

    auto json_sn = Utility::json_from_sn(sn);

    std::ifstream csv_fs;
    auto const home_path = Utility::get_environmental_variable("HOME");
    auto const csv_dir_path = home_path + std::string("/station_files");
    csv_fs.open(csv_dir_path + "/good.csv");

    int port_remote = 3330;
    std::string line;
    while ( std::getline(csv_fs, line) ) {

        auto const tokens = Utility::get_tokens(line);
        if (tokens.size() < 9) throw std::logic_error("add_to_config <8 tokens");

        // get properties from line
        auto const & station_name = tokens[0];
        auto const & model = tokens[3];
        auto const q_index = std::stoi(tokens[4]);
        auto const & input = tokens[5];
        auto const & ip_remote = tokens[6];
        auto const port_host = std::stoi(tokens[7]);
        auto const & serial_number = tokens[8];
        auto const & auth_code = tokens[9];

        std::cout << std::endl;
        std::cout << station_name << " " << model << " " << input
                  << " ip_remote:_" << ip_remote << "_";

        auto add_s = [&](auto & q) {
            auto const s_json = Utility::json_add_s(input, model, model);
            q.s.push_back( Utility::s_from_json(s_json) );
        };

        auto add_q = [&](auto & st) {
            port_remote++;
            auto const q_json = Utility::json_add_q(serial_number,
                                                    ip_remote,
                                                    port_remote,
                                                    auth_code,
                                                    port_host);
            st.q.push_back( Utility::q_from_json(q_json) );
        };

        if ( sn.has_station(station_name) ) {
            auto & st = sn.st_ref(station_name);
            if ( q_index < st.q.size() ) {
                // best case, only add the sensor
                add_s(st.q[q_index]);
            } else {
                // add digitizer and sensor
                add_q(st);
                add_s( st.q.back() );
            }

        } else {
            auto const st_json = Utility::json_add_st(station_name);
            sn.st.push_back( Utility::st_from_json(st_json) );
            auto & st = sn.st.back();
            add_q(st);
            add_s( st.q.back() );
        }
    }

    Utility::save_to_config_file(sn, config_file_path);
}

} // end namespace
