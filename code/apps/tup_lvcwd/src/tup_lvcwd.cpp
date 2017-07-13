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

    try {

        auto const port = "/dev/ttyS3";
        auto constexpr boudrate = 115200;
        auto constexpr timeout = std::chrono::milliseconds(500);
        mzn::SerialConnection sc(port, boudrate, timeout);
        sc.setup_connection();

        using Message = std::string;
        auto constexpr ret_size = 548;
        Message sc_msg_recv = "";
        sc_msg_recv.resize(ret_size);
        auto const msg = "sn?";
        Message sc_msg_send = msg + std::string("\r");
        sc.send_recv(sc_msg_send, sc_msg_recv);
        std::cout << std::endl << "[" << sc_msg_recv << "]\n";

    } catch (std::exception & e) {

        std::cerr << "\nunexpected error, closing program";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};


