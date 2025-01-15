#pragma once
#include <vector>
#include <string>
#include "TempEntry.h"
#include "Candlestick.h"


class FilterData {
public:
    // Filter candlestick data by temperature range
    static std::vector<Candlestick> filterCandlesticksByTemperature(const std::vector<Candlestick>& candlesticks, double minTemperature, double maxTemperature);

    // Filter candlestick data by country code
    static std::vector<Candlestick> filterCandlesticksByCountryCode(const std::vector<Candlestick>& candlesticks, const std::string& countryCode);
    // Filter candlestick data by date range
    static std::vector<Candlestick> filterCandlesticksByDateRange(
    const std::vector<Candlestick>& candlesticks, 
    const std::string& startDate, 
    const std::string& endDate);

}; 
