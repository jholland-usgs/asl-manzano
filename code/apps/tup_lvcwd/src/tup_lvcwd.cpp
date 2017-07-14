// Green Manzano
#include <iostream>
#include <vector>
#include <exception>
#include <cstdlib>
#include "serial_connection.h"
#include <thread>

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {

    std::cout << "\n +++++++++++++++++++++++++++ "
              << "\n +           TUP           + "
              << "\n +  Low Voltage Condition  + "
              << "\n +        Watchdog         + "
              << "\n +++++++++++++++++++++++++++ " << std::endl;

    using Message = std::string;
    auto const port = "/dev/ttyS2";
    auto constexpr boudrate = 115200;
    auto constexpr timeout = std::chrono::milliseconds(300);
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
        };

        sc_send_recv("\n");

        while (true) {

            std::cout << std::flush << "_";

            auto response = sc_send_recv("sddc?");

            // most likely case, all normal
            if (response.find("Disable") != std::string::npos) {
                std::cout << ".";
                std::this_thread::sleep_for( std::chrono::milliseconds(500) );
                continue;
            }

            // something else..

            try {

                // check if format is correct
                std::string const title = "SDDC = ";
                auto const title_index = response.find(title);

                if (title_index == std::string::npos) {
                    std::cerr << std::endl << "!bad response!";
                    // don't throw
                    continue;
                }

                // title was found
                auto const value_index = title_index + title.size();
                auto const value_and_extra = response.substr(value_index);
                std::istringstream iss;
                iss.str(value_and_extra);
                std::string value_string;
                std::getline(iss, value_string);

                if ( value_string.empty() ) {
                    std::cerr << std::endl << "!bad value_string!";
                    // don't throw
                    continue;
                }

                // format looks good

                // since Disable has not been found, value is the time to shutdown
                auto const value = std::stoi(value_string);
                std::cout << std::endl << "tup will shutdown! | time:" << value;
                std::cout << std::endl;
                std::cout << std::endl << "computer will shutdown!" << std::endl;
                std::this_thread::sleep_for( std::chrono::seconds(1) );
                std::cout << std::endl << "bye!" << std::endl;
                std::system("sudo shutdown now");
                break;

            } catch (...) {
                std::cerr << "could not find value in string";
            }
        }

    } catch (std::exception & e) {

        std::cerr << "\nunexpected error, closing program";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};


