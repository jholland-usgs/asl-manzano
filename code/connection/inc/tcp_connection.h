// ** TcpConnection Object ** //
// Manzano software

#ifndef _MZN_TCP_CONNECTION_H
#define _MZN_TCP_CONNECTION_H

// Unix sockets imports
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

// Utility
#include <cstring>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "message.h"
#include "address_info_handler.h"
#include "mzn_except.h"

namespace mzn {

//! Takes a msg and send/recv using an tcp connection
/*!
    Simple TCP connection, using posix tcp sockets
    implemented on plain C.

    internet socket address
    SockAddr defined with AddressInfoHandler same as linux sockaddr_in

    IMPORTANT:
        Updates the socket address of the remote when a message
        from remote is received. This means that if the tcp connection
        is used to listen first, the remote ip/port in the constructor
        does not get used. This is intended, since sometimes the connection
        does not know the ip/port of the remote.
        TODO: refactor nicely to show this in code (different constructors?)d

    @tparam M message data countainer type.
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class TcpConnection {

public:

    //! if the remote is not known, it will be set to the remote of recv msgs
    TcpConnection(int const port_host) : port_host_(port_host) {}

    ~TcpConnection() {if (skfd_ > 0) close_socket();}

    //! setups posix TCP socket
    //! When the connections recv will timeout, nothing for no timeout
    //! timeval is a posix time duration type
    void setup_socket(TimeoutDuration const & timeout = TimeoutDuration{},
                      std::string const & ip_remote = "",
                      int port_remote = 0);

    //! calls low level close() and shutdown() on the socket
    void close_socket();
    void shutdown_socket();

    //! utility to print array/vector type objects, if using std::string
    //! it would be better to just use their operator<<
    template< typename M = std::vector<uint8_t> >
    static
    void print_msg(M const & msg);

    //! @throws InfoException
    template< typename M = std::vector<uint8_t> >
    void send(M const & msg);

    //! @throws InfoException
    //! calls send and then recv
    template< typename SendType = std::vector<uint8_t> ,
              typename RecvType = std::vector<uint8_t> >
    void send_recv(SendType const & msg_send, RecvType & msg_recv);

    //! @throws InfoException
    template< typename M = std::vector<uint8_t> >
    void recv(M & msg_recv);

    //! accessors to internal current values
    int port_remote() const {return sin_remote_.sin_port;}
    //! make into string? TODO: allow for ipv6
    uint32_t ip_remote() const {return sin_remote_.sin_addr.s_addr;}
    int port_host() const {return port_host_;}

private:

    //! socket file descriptor
    int skfd_ = 0;

    //! true this when responding to remote of last received messages
    //! set in setup_socket() if no port_remote  is given
    bool should_update_sin_remote_ = false;

    //! this holds the information of the remote computer
    //! as reported when a message is received
    SockAddr sin_remote_recv_;

    //! this holds the information of the remote computer
    //! set first according to the constructor ip/port remote
    SockAddr sin_remote_;

    //! information about connection, only used in setup_socket
    //! port numbers in big endian
    int const port_host_;

    //! print debug messages
    bool debug_ = false;
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<< (std::ostream & tc_os,
                           TcpConnection const & tc) {

    tc_os << "tc[" << tc.port_remote() << "]";
    return tc_os;
};

// -------------------------------------------------------------------------- //
template<typename M>
void TcpConnection::print_msg(M const & msg) {
    std::cout << std::uppercase <<std::showbase << std::hex << "\n{";
    for (auto const & b : msg) std::cout << static_cast<const int>(b) << ", ";
    std::cout << "}\n" << std::noshowbase << std::uppercase << std::dec;
}

// -------------------------------------------------------------------------- //
template<typename SendType, typename RecvType>
void TcpConnection::send_recv(SendType const & msg_send,
                              RecvType & msg_recv) {
    send(msg_send);
    recv(msg_recv);
}

// -------------------------------------------------------------------------- //
template<typename M>
void TcpConnection::send(M const & msg) {

    // send message
    if (debug_) std::cout << "\nsending message\n";

    // sendto signature from linux man
    // ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
    //                const struct sockaddr *dest_addr, socklen_t addrlen);

    // ssize_t : C style signed std::size_t, since it could give neg error nums

    // requires sin_remote_ to be set in the constructor or a recv message
    if (sin_remote_.sin_port == 0) {
        throw std::logic_error("TcpConnection Remote port/ip has not been set");
    }

    // send is a c function, does not throw, provides an error code in the
    // send result if there was an error
    // int send(int sockfd, const void *msg, int len, int flags);
    ssize_t send_result = ::send(skfd_,
                                 &msg[0],
                                 msg.size(),
                                 0);

    // Failure
    if (send_result < 0) {

        std::string const msg_error = std::string("error in sending msg: \n")
                                      + std::strerror(errno);

        throw InfoException("TcpConnection", "send", msg_error);
    }

    // Success!
    if (debug_) {
        std::cout << "\nsent bytes: " << send_result << "\n";
        print_msg(msg);
    }
}

// -------------------------------------------------------------------------- //
template<typename M>
void TcpConnection::recv(M & msg_recv) {

    int recv_result = ::recv(skfd_,
                             &msg_recv[0],
                             msg_recv.size(),
                             0);

    // Failure
    if (recv_result < 0) {

        std::string const msg_error = std::string("error in receiving msg: ")
                                      + std::strerror(errno);

        throw InfoException("TcpConnection", "recv", msg_error);
    }

    // Success!

    // if no remote port/ip given at setup_socket
    // use the information from this message as the remote
    if (should_update_sin_remote_) sin_remote_ = sin_remote_recv_;

    // resize the msg to the number of bytes received
    msg_recv.resize(recv_result);

    if (debug_) {

        std::cout << "\nreceived bytes: " << recv_result << std::endl;

        print_msg(msg_recv);

        std::cout << "\ngot msg from: "
                  << std::hex << sin_remote_recv_.sin_addr.s_addr
                  << " at port: " << sin_remote_recv_.sin_port
                  << std::dec;
    }
}

} // end mzn
#endif // _MZN_TCP_CONNECTION_H_
