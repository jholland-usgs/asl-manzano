// Manzano Software //
#ifndef _MZN_STREAM_OUTPUT_H_
#define _MZN_STREAM_OUTPUT_H_

#include <vector>
#include <string>
#include <sstream>

#include "seismic_network.h"
#include "instruction_map.h"
#include "c1_qcal.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class StreamOutput {

    SeismicNetwork const & sn_;

public:

    explicit
    StreamOutput(SeismicNetwork const & sn) :
            sn_(sn) {}

    ~StreamOutput() = default;

    template <Kind kind>
    void show(TargetAddress const & ta) const {
        auto constexpr ui_id = UserInstruction::hash(Action::show, kind);
        std::cerr << "\n___SHOW general case___: " << ui_id;
        throw std::logic_error("no show function for this kind");
    }

    void show_prompt(TargetAddress const & ta) const;

    void show_tree(std::vector<std::string> const & items,
                   std::vector<bool> const & margins = {}) const;

    void show_num_tree(std::vector<std::string> const & items) const;

    void show_help() const;

    // --------------------------------------------------------------------- //
    template<typename C>
    std::ostream & line_stream(std::ostream & os) const;

private:

    std::ostream & os = std::cout;

    void show_margins(std::vector<bool> const & margins) const;

    void show_target() const;

    void show_target(Station const & st,
                     unsigned const target_index,
                     std::vector<bool> const & margins = {}) const;

    void show_target(Digitizer const & q,
                     unsigned const target_index,
                     std::vector<bool> const & margins = {}) const;

    void show_target(DataProcessor const & dp,
                     unsigned const target_index,
                     std::vector<bool> const & margins = {}) const;

    void show_target(Sensor const & s,
                     unsigned const target_index,
                     std::vector<bool> const & margins = {}) const;
};

// --------------------------------------------------------------------- //
template<>
inline
std::ostream & StreamOutput::line_stream<C1Qcal>(std::ostream & os) const {
    return os;
}

// -------------------------------------------------------------------------- //
template<>
inline
void StreamOutput::show<Kind::target>(TargetAddress const & ta) const {

    // this function is called only with full targets
    Target const t = ta.target();

    switch (t.scope) {

        case Scope::seismic_network:    show_target(); break;

        case Scope::station: {
            auto & st = sn_.st_const_ref(ta); show_target(st, t.index); break;
        }
        case Scope::digitizer: {
            auto & q  = sn_.q_const_ref(ta);  show_target(q, t.index);  break;
        }
        case Scope::sensor: {
            auto & s  = sn_.s_const_ref(ta);  show_target(s, t.index);  break;
        }
        case Scope::data_processor: {
            auto & dp = sn_.dp_const_ref(ta); show_target(dp, t.index); break;
        }
        default: break;
    }
}

// -------------------------------------------------------------------------- //
template<>
inline
void StreamOutput::show<Kind::config>(TargetAddress const & ta) const {

    auto scope = ta.scope();

    using S = Scope;
    os << "\nConfig:\n";
    switch (scope) {
        case S::seismic_network: sn_.stream_config(os);                  break;
        case S::digitizer:       sn_.q_const_ref(ta).stream_config(os);  break;
        case S::station:         sn_.st_const_ref(ta).stream_config(os); break;
        case S::data_processor:  sn_.dp_const_ref(ta).stream_config(os); break;
        case S::sensor:          sn_.s_const_ref(ta).stream_config(os);  break;

        default: throw std::logic_error{"@StreamOutput::show_config"};
    }
}

// -------------------------------------------------------------------------- //
template<>
inline
void StreamOutput::show<Kind::status>(TargetAddress const & ta) const {

    auto scope = ta.scope();

    using S = Scope;
    os << "\nStatus:\n";

    switch (scope) {
        case S::seismic_network: sn_.stream_status(os);                  break;
        case S::digitizer:       sn_.q_const_ref(ta).stream_status(os);  break;
        case S::station:         sn_.st_const_ref(ta).stream_status(os); break;
        case S::data_processor:  sn_.dp_const_ref(ta).stream_status(os); break;
        case S::sensor:          sn_.s_const_ref(ta).stream_status(os);  break;

        default: throw std::logic_error{"@StreamOutput::show_status"};
    }

}

// show help
// -------------------------------------------------------------------------- //
template<>
inline
void StreamOutput::show<Kind::help>(TargetAddress const & ta) const {

    auto const actions = InstructionMap::filter_actions(ta);

    for (auto const action : actions) {

        os << "\n\n " << action;

        auto const kinds = InstructionMap::filter_kinds(ta, action);
        auto const n = kinds.size();

        for(unsigned i = 0; i < n; i++) {

            // -------- KINDS --------- //
            os << "\n ";
            if (i + 1 == n) os << "└── "; else os << "├── ";

            os << kinds[i];

            // ------- OPTIONS --------- //
            auto const options =
                InstructionMap::filter_options(action, kinds[i]);

            auto const m = options.size();

            if (m > 0) {

                // this command can be ran without setting an option
                if ( InstructionMap::has_empty_option(action, kinds[i]) ) {
                    os << " []";
                }

                for (unsigned j = 0; j < m; j++) {
                    os << " [:" << options[j] << "]";
                }

            }
        }
    }
}

} // end namespace

#endif
