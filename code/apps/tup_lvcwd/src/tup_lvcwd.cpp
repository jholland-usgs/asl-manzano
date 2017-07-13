// Green Manzano
#include <iostream>
#include <vector>
#include <exception>
#include <cstdlib>
#include "serial_connection.h"

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {

    std::cout << "\n +++++++++++++++++++++++++++ "
              << "\n +           TUP           + "
              << "\n +  Low Voltage Condition  + "
              << "\n +        Watchdog         + "
              << "\n +++++++++++++++++++++++++++ ";

    using Message = std::string;
    auto const port = "/dev/ttyS2";
    auto constexpr boudrate = 115200;
    auto constexpr timeout = std::chrono::milliseconds(500);
    auto constexpr ret_size = 548;

    try {

        mzn::SerialConnection sc(port, boudrate, timeout);
        sc.setup_connection();

        auto sc_send_recv = [&sc](auto const msg) {
            Message sc_msg_recv = "";
            sc_msg_recv.resize(ret_size);
            Message sc_msg_send = msg + std::string("\r");
            sc.send_recv(sc_msg_send, sc_msg_recv);
            // std::cout << std::endl << "[" << sc_msg_recv << "]\n";
            return sc_msg_recv;
        }

        auto const response = sc_send_recv("sn?");
        std::cout << std::endl << "response:[" << response << "]";

    } catch (std::exception & e) {

        std::cerr << "\nunexpected error, closing program";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};


