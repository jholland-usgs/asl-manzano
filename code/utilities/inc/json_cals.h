#ifndef _MZN_JSON_CALS_H_
#define _MZN_JSON_CALS_H_

#include "json_mzn.h"
#include "system_calls.h"

namespace mzn {
namespace Utility {

    std::string cal_line(Json const & cal_json);
    std::vector<std::string> cals_lines(Json const & cals_json);
    void check_cal_json(Json const & cal);
    Json get_cals_json(std::string const & cals);

} // <- Utility
} // <- mzn

#endif
