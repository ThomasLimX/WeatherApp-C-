#include "TempEntry.h"

// TempEntry constructor implementation
TempEntry::TempEntry(
    const std::string& country,
    const std::string& date,
    double open,
    double high,
    double low,
    double close
) : country(country), date(date), open(open), high(high), low(low), close(close) {}
