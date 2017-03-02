#include "cx_serial_sensor_status.h"
namespace mzn {
uint16_t CxSerialSensorStatus::number_of_ic(std::vector<uint8_t> const & msg,
                                            uint16_t mf_begin) const {

   return this->number_of_sub_blocks();

}
}
