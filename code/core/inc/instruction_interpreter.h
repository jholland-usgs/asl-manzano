// Manzano Software //
#ifndef _MZN_INSTRUCTION_INTERPRETER_H_
#define _MZN_INSTRUCTION_INTERPRETER_H_

#include <iostream>
#include <map>
#include <sstream>
#include <functional>

#include "string_utilities.h"
#include "comm.h"

namespace mzn {

//! Takes a run time ui calls the right template functions in Comm
/*
    Optionally takes a TargetAddress
    @see UserInput (ui)
    @see TargetAddress (ta)
    In addition provides checks to make sure the the ui/ta
    combination is correct
    @author rfigueroa@usgs.gov
 */
class InstructionInterpreter {

public:
    explicit
    InstructionInterpreter(
            TargetAddress const & ta =
                TargetAddress{Target(Scope::station, 0),
                              Target(Scope::digitizer,  0)});

    ~InstructionInterpreter() = default;

private:

    //! current initialized to first digitizer in the sn
    TargetAddress current_ta_;

public:

    //! Comm runs the instructions and stores outputs
    Comm cm;

    //! metaprogramming to do the call of action/kind combinations
    template<Action action>
    void match(UserInstruction const & ui,
               TargetAddress const & ta);

    //! @calls functions in Comm
    //! called from green_manzano (user_interpreter)
    //! or from red_manzano (window)
    void run_instruction(UserInstruction const & ui,
                         TargetAddress const & ta);

    //! If a TargetAddress is not specified, this overload
    //! runs the default run_instruction(ui,ta) with the current_ta_
    void run_instruction(UserInstruction const & ui);

    //! called from green_manzano (user_interpreter)
    //! @throws warning if there is a bad action/kind/target_address combination
    //! does nothing otherwise
    void check_instruction_map(UserInstruction const & ui,
                               TargetAddress const & ta) const;

    //! @calls check_instruction_map(ui, current_ta_);
    void check_instruction_map(UserInstruction const & ui) const;

    void show_prompt() const;

    //! called from green_manzano (user_interpreter::run_user_input)
    //! check that the ta is complete or well formed
    void merge_and_check_target_address(TargetAddress & ta) const;

    //! and that all the elements in ta exist in the seismic network
    void check_ta_in_sn(TargetAddress const & ta) const;

    //! move one level up in the hierchical tree of the current_ta
    void current_ta_remove_one_target();
};

// -------- //

// -------------------------------------------------------------------------- //
template<Action action>
inline
void InstructionInterpreter::match(UserInstruction const & ui,
                                   TargetAddress const & ta) {

    auto const & oi = ui.option_input;

    switch (ui.kind) {
        case Kind::target:  cm.run<action, Kind::target>(ta, oi);  break;
        case Kind::config:  cm.run<action, Kind::config>(ta, oi);  break;
        case Kind::status:  cm.run<action, Kind::status>(ta, oi);  break;
        case Kind::help:    cm.run<action, Kind::help>(ta, oi);    break;
        case Kind::poll:    cm.run<action, Kind::poll>(ta, oi);    break;
        case Kind::global:  cm.run<action, Kind::global>(ta, oi);  break;
        case Kind::ping:    cm.run<action, Kind::ping>(ta, oi);    break;
        case Kind::stat:    cm.run<action, Kind::stat>(ta, oi);    break;
        case Kind::qview:   cm.run<action, Kind::qview>(ta, oi);   break;
        case Kind::dev:     cm.run<action, Kind::dev>(ta, oi);     break;
        case Kind::ctrl:    cm.run<action, Kind::ctrl>(ta, oi);    break;
        case Kind::reg:     cm.run<action, Kind::reg>(ta, oi);     break;
        case Kind::dereg:   cm.run<action, Kind::dereg>(ta, oi);   break;
        case Kind::cal:     cm.run<action, Kind::cal>(ta, oi);     break;
        case Kind::center:  cm.run<action, Kind::center>(ta, oi);  break;
        case Kind::uptime:  cm.run<action, Kind::uptime>(ta, oi);  break;
        case Kind::mzn:     cm.run<action, Kind::mzn>(ta, oi);     break;
        case Kind::plan:    cm.run<action, Kind::plan>(ta, oi);    break;
        case Kind::link:    cm.run<action, Kind::link>(ta, oi);    break;
        case Kind::token:   cm.run<action, Kind::token>(ta, oi);   break;
        case Kind::wait:    cm.run<action, Kind::wait>(ta, oi);    break;
        case Kind::output:  cm.run<action, Kind::output>(ta, oi);  break;
        default:
            throw std::logic_error{"at InstructionInterpreter::run_instruction"};
    }
}
} // end namespace
#endif
