#pragma once
#include <string>
#include <vector>
#include <map>
#include "Candlestick.h"

class TempEntry {
public:
    std::string country;
    std::string date;
    double open;
    double high;
    double low;
    double close;

    // Constructor
    TempEntry(const std::string& country, const std::string& date, double open, double high, double low, double close);
};
