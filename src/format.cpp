#include <string>
#include <chrono>
#include <sstream>
#include "format.h"


using std::string;

// string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }

string Format::ElapsedTime(long seconds) { 

    std::ostringstream st;

    int h = seconds/3600;
    int m = ((seconds%3600)/60);
    int s = ((seconds%3600)%60);

    st.width(2);
    st.fill('0');
    st<<h<<":";
    st.width(2);
    st.fill('0');
    st<<m<<":";
    st.width(2);
    st.fill('0');
    st<<s;
    string result = std::to_string(seconds/3600)+":"+std::to_string((seconds%3600)/60)+":"+std::to_string(((seconds%3600)%60));
    return st.str(); }

//cpp 20 method
// string Format::ElapsedTime(long seconds) { 
//     std::chrono::hh_mm_ss time{std::chrono::seconds(seconds)};
//     string result = std::to_string(time.hours().count())+":"+std::to_string(time.minutes().count())+":"+std::to_string(time.seconds().count());
//     return result; }
