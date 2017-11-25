#pragma once

#include <string>
#include <sstream>

namespace roboclaw
{
namespace io
{

struct pid_values
{
    float p;
    float i;
    float d;
    uint32_t qpps;
};

inline std::string get_string(const pid_values& v)
{
    std::stringstream ss;
    ss << "p: " << v.p << ", i: " << v.i << ", d: " << v.d << ", qpps: " << v.qpps;
    return ss.str();
}

struct encoder_count
{
    uint16_t count;
    struct _status
    {
        uint8_t underflow : 1;
        uint8_t overflow : 1;
        uint8_t forward : 1;
    } status;
};

inline std::string get_string(const roboclaw::io::encoder_count& m)
{
    std::stringstream ss;
    ss << "count: " << m.count << " ["
       << (m.status.underflow ? "underflow," : "")
       << (m.status.overflow ? "overflow," : "")
       << (m.status.forward ? "forward" : "backward")
       << "]";

    return ss.str();
}

/* ------------------------------------------------------------------------- */

struct encoder_mode
{
    uint8_t quadrature : 1;
    uint8_t enable_rc_analog : 1;
};

struct encoder_mode_1_2
{
    encoder_mode enc1;
    encoder_mode enc2;
};

inline std::string get_string(const encoder_mode_1_2& m)
{
    auto print_one = [] (const std::string& heading,
                         const roboclaw::io::encoder_mode& m)
    {
        std::stringstream ss;
        ss << heading << (m.quadrature ? "quadrature" : "absolute")
                      << (m.enable_rc_analog ? ", rc/analog enabled" : "");
        return ss.str(); 
    };

    std::string result = "";
    result += print_one("encoder1: ", m.enc1) + "; " + print_one("encoder2: ", m.enc2);

    return result;
}


} /* io */ 
} /* roboclaw */ 
