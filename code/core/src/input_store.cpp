// Manzano software
#include "input_store.h"
namespace mzn {

// -------------------------------------------------------------------------- //
InputStore::InputStore(SeismicNetwork const & sn) : sn_(sn) {}

// -------------------------------------------------------------------------- //
std::map< uint16_t, InputStore::OptionCmdMap > InputStore::mpid_options_map;

} // <- mzn
