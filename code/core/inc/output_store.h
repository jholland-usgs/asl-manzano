// ** ResultStore Object ** //
// Manzano software
#ifndef _MZN_OUTPUT_STORE_H_
#define _MZN_OUTPUT_STORE_H_

#include <unordered_map>
#include <memory>
#include "commands.h"
#include "user_instruction.h"
#include "target_address.h"
#include "seismic_network.h"
#include "ui_to_cmd_type.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class OutputStore {

public:

    explicit
    OutputStore() = default;
    ~OutputStore() = default;

    // --------------------------------------------------------------------- //
    std::unordered_map< uint32_t, std::unique_ptr<Command> > cmd_map;

    // --------------------------------------------------------------------- //
    template <Action action, Kind kind>
    inline
    bool contains(TargetAddress const & ta) {
        auto constexpr ui_id = UserInstruction::hash(action, kind);
        auto const task_id = ta.hash() + ui_id;
        auto const it = cmd_map.find(task_id);
        if ( it != cmd_map.end() ) return true;
        else return false;
    }

    // --------------------------------------------------------------------- //
    template <Action action, Kind kind,
              class Co = typename Co<action, kind>::type >
    inline
    std::unique_ptr<Co> pop_output_cmd(TargetAddress const & ta) {

        auto constexpr ui_id = UserInstruction::hash(action, kind);
        auto const task_id = ta.hash() + ui_id;
        auto it = cmd_map.find(task_id);
        // find and dynamic cast to Co type
        Co * cmd;
        if( it != cmd_map.end() ) {
            cmd = dynamic_cast<Co *>( cmd_map[task_id].get() );
            if (cmd == nullptr) throw std::logic_error("@OutputStore::get_cmd");
        } else {
            throw WarningException("OutputStore",
                                   "get_cmd",
                                   "cmd not found in the output store");
        }
        // cmd is in a valid state and points to memory held by cmd_map[task_id]
        // release map unique_ptr and give ownership to unique_tr Co type
        cmd_map[task_id].release();
        std::unique_ptr<Co> cmd_casted(cmd);
        // map unique_ptr is nullptr, remove from map
        cmd_map.erase(it);
        // give ownership to caller
        return std::move(cmd_casted);
    }

    // TODO: not currently used, test with usage, intended to copy Co
    // --------------------------------------------------------------------- //
    template <Action action, Kind kind,
              class Co = typename Co<action, kind>::type >
    inline
    Co get_output_cmd(TargetAddress const & ta) {

        auto constexpr ui_id = UserInstruction::hash(action, kind);
        auto const task_id = ta.hash() + ui_id;
        auto search_result = cmd_map.find(task_id);

        Co * cmd;

        if( search_result != cmd_map.end() ) {
            cmd = dynamic_cast<Co *>( cmd_map[task_id].get() );
            if (cmd == nullptr) throw std::logic_error("@OutputStore::get_cmd");
        } else {
            throw WarningException("OutputStore",
                                   "get_cmd",
                                   "cmd not found in the output store");
        }

        return *cmd;
    }

    //! An option when the specific command type is not needed
    //! and action/kind is not known at compile time
    //! only the output of the command is needed
    // --------------------------------------------------------------------- //
    std::string get_string_output(TargetAddress const & ta,
                                  UserInstruction const & ui) {

        auto const ui_id = UserInstruction::hash(ui.action, ui.kind);
        auto const task_id = ta.hash() + ui_id;
        auto search_result = cmd_map.find(task_id);

        if( search_result != cmd_map.end() ) {

            // command found, return the print output
            std::stringstream ss;
            ss << *( cmd_map[task_id].get() );
            return ss.str();

        } else {
            throw WarningException("OutputStore",
                                   "get_cmd",
                                   "cmd not found in the output store");
        }
    }

};

} // end namespace

#endif // _MZN_OUTPUT_STORE_H_
