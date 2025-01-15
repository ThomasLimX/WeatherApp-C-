#include "FilterData.h"
#include <iostream>
#include <iomanip>
#include <algorithm> 
#include <vector>
#include <numbers>
#include <numeric>

//Filter Temperature
std::vector<Candlestick> FilterData::filterCandlesticksByTemperature(
    const std::vector<Candlestick>& candlesticks, double minTemperature, double maxTemperature) 
{
    std::vector<Candlestick> filtered;

    // Copy candlesticks fully within the temperature range
    std::copy_if(candlesticks.begin(), candlesticks.end(), std::back_inserter(filtered),
                 [minTemperature, maxTemperature](const Candlestick& candle) {
                     return (candle.high <= maxTemperature && candle.low >= minTemperature);
                 });

    return filtered;
}





//Filter CountryCode
std::vector<Candlestick> FilterData::filterCandlesticksByCountryCode(const std::vector<Candlestick>& candlesticks, const std::string& countryCode) {
    std::vector<Candlestick> filtered;

    // Filter candlesticks by country code
    std::copy_if(candlesticks.begin(), candlesticks.end(), std::back_inserter(filtered),
                 [&countryCode](const Candlestick& candle) {
                     return candle.country == countryCode;
                 });

    return filtered;
}

//Filter Date
std::vector<Candlestick> FilterData::filterCandlesticksByDateRange(
    const std::vector<Candlestick>& candlesticks, 
    const std::string& startDate, 
    const std::string& endDate) 
{
    std::vector<Candlestick> filtered;

    // Filter candlesticks within the date range
    std::copy_if(candlesticks.begin(), candlesticks.end(), std::back_inserter(filtered),
                 [&startDate, &endDate](const Candlestick& candle) {
                     return candle.date >= startDate && candle.date <= endDate;
                 });

    return filtered;
}










