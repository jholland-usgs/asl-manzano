// ** message_dispatch Object ** //
// Manzano software

#ifndef _MZN_MESSAGE_DISPATCH_H
#define _MZN_MESSAGE_DISPATCH_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <iomanip>
#include <bitset>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>
#include <chrono>
#include <mutex>

#include <thread>

#include "md5.h"

#include "seismic_network.h"
#include "commands.h"

namespace mzn {

//! Takes a cmd send/recv and makes it into a msg send/recv
/*!
    handles the basic errors when communicating with the digitizer
    manages the creation of the qpd header, including the crc calculation
    handles timed_send messages which the autocal_scheduler uses
    called md or md_ in the classes that use it
    @author rfigueroa@usgs.gov
 */
class MessageDispatch {

//! md class is an object of Comm
friend class Comm;

public:
    //! setups the threads for timed_send
    // --------------------------------------------------------------------- //
    MessageDispatch() = default;
    ~MessageDispatch() = default;

    //! can't copy
    // --------------------------------------------------------------------- //
    MessageDispatch(MessageDispatch const & md) = delete;
    MessageDispatch & operator=(MessageDispatch const & md) = delete;

    // --------------------------------------------------------------------- //
    void send_recv(ConnectionHandler & connection_handler,
                   Command const & cmd_send,
                   Command & cmd_recv,
                   bool const print = true);

    // --------------------------------------------------------------------- //
    void data_recv(Digitizer & q,
                   DtOpen const & cmd_send,
                   Qdp::Message & msg_recv,
                   bool const print = true) const;

private:

    //! locking send_recv so that only one thread can access it
    std::mutex send_recv_mutex_;

    //! constants of the current version of the digitizer qdp
    //! qdp: Quanterra Data Packet
    uint16_t static constexpr qdp_begin_crc_ = 0;
    uint16_t static constexpr qdp_begin_header_ = 4;
    uint16_t static constexpr qdp_begin_cmd_data_ = 12;

    //! size of the qdp_message for cerr
    uint16_t static constexpr cerr_qdp_message_size_ = 14;

    //! mtu: digitizer maximum transferable unit
    uint16_t static constexpr mtu_ = 576;

    //! cmd -> qdp_message
    /*! creates the qdp_message with the qdp header
        calls crc_calc and sets the crc
        if (to_send) connection_handler.inc_sequence_number()
        uses: connection_handler.firmware_version,
              connection_handler.sequence_number(),
              connection_handler.acknowledge_number()
              in creating the qdp header
     */
    Qdp::Message create_qdp_msg(ConnectionHandler & connection_handler,
                                Command const & cmd,
                                bool const to_send = true) const;

    //! creates CRC for new qdp_messages and checking received messages
    template <typename M>
    static
    std::array<uint8_t, 4> calc_crc(M const & msg);


    // TODO: move to constexpr as it was
    static
    std::array<uint32_t const, 256> const crc_table_;
};

// utility private methods
// -------------------------------------------------------------------------- //
template <typename M>
inline
std::array<uint8_t, 4>
MessageDispatch::calc_crc(M const & msg) {

    uint32_t constexpr mask_lsbyte(0xFF);

    uint32_t crc_msbyte = 0;
    uint32_t crc = 0;
    uint32_t crc_table_index = 0;

    // for every byte on the message, create/update the
    // cyclic redundancy code
    // to be sent to the digitizers
    // the code follows the digitizer's communication protocol manual
    // and dp writer's manual

    for (int i = qdp_begin_header_; i < msg.size(); i++) {

        uint32_t const msg_byte( msg[i] );
        crc_table_index = (crc_msbyte ^ msg_byte) & mask_lsbyte;
        crc = (crc << 8) ^ crc_table_[crc_table_index];
        crc_msbyte = crc >> 24;
    }

    std::array<uint8_t, 4> crc_array;
    crc_array[0] = static_cast<uint8_t>( (crc >> 24) & mask_lsbyte );
    crc_array[1] = static_cast<uint8_t>( (crc >> 16) & mask_lsbyte );
    crc_array[2] = static_cast<uint8_t>( (crc >> 8)  & mask_lsbyte );
    crc_array[3] = static_cast<uint8_t>(crc & mask_lsbyte);

    return crc_array;
}
} // end namespace
#endif // _MZN_MESSAGE_DISPATCH_H_
