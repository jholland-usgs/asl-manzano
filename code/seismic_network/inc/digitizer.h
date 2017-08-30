// Manzano software
#ifndef _MZN_DIGITIZER_H
#define _MZN_DIGITIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <atomic>

#include "sensor.h"
#include "mzn_except.h"

namespace mzn {

//! Holds configuration and status of digitizer
/*!
    In addition, has a unique udp connection
    and the sensor targets
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class Digitizer {

private:
    struct Config {

        friend class Digitizer;

        Config(uint64_t const serial_number,
               std::string const model = "Q330") :

            serial_number(serial_number),
            model(model) {
        }

        ~Config() = default;

        uint64_t const serial_number;
        unsigned int kmi_tag = 0;
        std::string const model;
    };

public:
    //! currently using port_config due to current mzn functionality
    //! the configuration with data/control works if mzn needs data directly
    explicit
    Digitizer(uint64_t const serial_number,
              ConnectionHandler port_config) :

    port_config( std::move(port_config) ),
    config(serial_number) {}

    //! check sensor configuration with digitizer model, currently 2 inputs only
    void check() const {
        if (s.size() < 2) return;
        // A/B good, A/A bad, B/B bad
        if (s.size() == 2 and s[0].config.input != s[1].config.input) return;
        // bad
        for (auto const & s_ : s) std::cerr << s_;
        throw FatalException("Digitizer",
                             "check",
                             "sensor configuration conflict");
    }
    ~Digitizer() = default;

public:
    //! digitizer ports, each one with an udp connection
    ConnectionHandler port_config;

    //! sensor targets
    std::vector<Sensor> s;
    //! configuration set at construction
    Config const config;

    // move constructor
    // ---------------------------------------------------------------------- //
    Digitizer(Digitizer && rhs) noexcept :
            port_config(  std::move(rhs.port_config) ),
            s( std::move(rhs.s) ),
            config(rhs.config) {}

    // ---------------------------------------------------------------------- //
    void stream_config(std::ostream & os) const;
    void stream_status(std::ostream & os) const;
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & q_os, Digitizer const & q) {

    q_os << "digitizer("
         <<  std::hex << std::uppercase
         << q.config.serial_number
         <<  std::dec << std::nouppercase
         << ")";
    return q_os;
}

} // end mzn
#endif // _MZN_DIGITIZER_H_
