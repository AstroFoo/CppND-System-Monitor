#include <string>

#include "format.h"

using std::string, std::to_string;

string FillZeroes(string s)
{
    if (s.size() == 0)
        return "00";
    else if (s.size() == 1)
        return "0" + s;
    return s;
}

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds)
{
    int hours = seconds/3600;
    int hoursInSecs = 3600*hours;
    int mins = (seconds-hoursInSecs)/60;
    int minsinSecs = 60*mins;
    int secs = seconds - hoursInSecs - minsinSecs;

    string elapsedHours = FillZeroes(to_string(hours));
    string elapsedMins = FillZeroes(to_string(mins));
    string elapsedSecs = FillZeroes(to_string(secs));

    return elapsedHours + ":" + elapsedMins + ":" + elapsedSecs;
}

