#include "Candlestick.h"
#include "TempEntry.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <numeric>
#include <stdexcept>
#include <set>

// Default Constructor
Candlestick::Candlestick() : country(""), date(""), open(0.0), high(0.0), low(0.0), close(0.0) {}

// Parameterized Constructor
Candlestick::Candlestick(const std::string& country, const std::string& date, double open, double high, double low, double close)
    : country(country), date(date), open(open), high(high), low(low), close(close) {}

// Tokenize CSV line
std::vector<std::string> Candlestick::tokenise(const std::string& csvLine, char separator) {
    std::vector<std::string> tokens;
    std::stringstream ss(csvLine);
    std::string token;

    while (std::getline(ss, token, separator)) {
        tokens.push_back(token);
    }
    return tokens;
}

//This is the old CSV reader

std::vector<TempEntry> Candlestick::readOldCSV(const std::string& csvFile) {
    std::ifstream file(csvFile);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open CSV file.");
    }

    std::vector<TempEntry> entries;
    std::string line;

    // Read the header to determine column indices
    std::getline(file, line);
    auto headers = tokenise(line, ',');
    std::map<std::string, int> countryTempIndices;

    // Find temperature columns for each country
    for (size_t i = 0; i < headers.size(); ++i) {
        if (headers[i].find("_temperature") != std::string::npos) {
            std::string country = headers[i].substr(0, 2); // Extract country code
            countryTempIndices[country] = i;
        }
    }

    // Process the data rows
    while (std::getline(file, line)) {
        auto tokens = tokenise(line, ',');
        if (tokens.size() < headers.size()) continue;

        std::string timestamp = tokens[0];

        // Process all rows, not just January 1st
        for (const auto& [country, index] : countryTempIndices) {
            double temperature = std::stod(tokens[index]);
            entries.emplace_back(country, timestamp, temperature, temperature, temperature, temperature);
        }
    }

    file.close();
    return entries;
}



std::vector<Candlestick> Candlestick::readCSV(const std::string& csvFile) {
    std::ifstream file(csvFile);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open CSV file.");
    }

    std::vector<Candlestick> candlesticks;
    std::string line;

    // Read and discard the header line
    std::getline(file, line);

    // Process the data rows
    while (std::getline(file, line)) {
        auto tokens = tokenise(line, ',');
        if (tokens.size() != 6) { // Ensure the row has the correct number of columns
            std::cerr << "Skipping invalid row: " << line << "\n";
            continue;
        }

        try {
            std::string country = tokens[0];
            std::string date = tokens[1];
            double open = std::stod(tokens[2]);
            double high = std::stod(tokens[3]);
            double low = std::stod(tokens[4]);
            double close = std::stod(tokens[5]);

            // Create a Candlestick object and add it to the vector
            candlesticks.emplace_back(country, date, open, high, low, close);
        } catch (const std::exception& ex) {
            std::cerr << "Error parsing row: " << line << ". Error: " << ex.what() << "\n";
        }
    }

    file.close();
    return candlesticks;
}

// Group temperatures by year
std::map<std::string, std::vector<double>> Candlestick::groupByYear(const std::vector<TempEntry>& entries) {
    std::map<std::string, std::vector<double>> yearlyData;

    for (const auto& entry : entries) {
        std::string year = entry.date.substr(0, 4);
        yearlyData[year].push_back(entry.open); // Assuming "open" holds the temperature
    }

    return yearlyData;
}



std::vector<Candlestick> Candlestick::computeCandlesticks(const std::vector<TempEntry>& entries, const std::string& filterCountry) {
    std::map<std::string, std::vector<double>> yearlyData;

for (const auto& entry : entries) {
    if (entry.country == filterCountry) {
        std::string year = entry.date.substr(0, 7);

        // Add all temperature fields without checking for duplicates
        yearlyData[year].push_back(entry.open);
        yearlyData[year].push_back(entry.high);
        yearlyData[year].push_back(entry.low);
        yearlyData[year].push_back(entry.close);
   

    }
}

    std::vector<Candlestick> candlesticks;
    double previousYearMean = 0.0; // For "open" of the next year

    for (const auto& [year, temperatures] : yearlyData) {
        if (temperatures.empty()) continue;

        // Compute High and Low from all grouped temperatures
        double high = *std::max_element(temperatures.begin(), temperatures.end());
        double low = *std::min_element(temperatures.begin(), temperatures.end());

        // Compute Open and Close
        double open = previousYearMean; // From the previous year's mean
        double close = std::accumulate(temperatures.begin(), temperatures.end(), 0.0) / temperatures.size();

        // Update previous year mean
        previousYearMean = close;

        // Add the candlestick
        candlesticks.emplace_back(filterCountry, year , open, high, low, close);
    }

    return candlesticks;
}


// Method to save candlestick data to a CSV file
std::vector<Candlestick> Candlestick::computeAllCandlesticks(const std::vector<TempEntry>& entries) {
    // Identify all unique countries in the data
    std::set<std::string> uniqueCountries;

    for (const auto& entry : entries) {
        uniqueCountries.insert(entry.country);
    }

    // Compute candlestick data for all countries
    std::vector<Candlestick> allCandlesticks;
    for (const auto& country : uniqueCountries) {
        std::vector<Candlestick> countryCandlesticks = computeCandlesticks(entries, country);
        allCandlesticks.insert(allCandlesticks.end(), countryCandlesticks.begin(), countryCandlesticks.end());
    }

    return allCandlesticks;
}

// Save Candlesitck data to CSV file
void Candlestick::saveToCSV(const std::vector<Candlestick>& candlesticks, const std::string& outputFile) {
    std::ofstream file(outputFile);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open output file.");
    }

    // Write CSV headers
    file << "Country,Date,Open,High,Low,Close\n";

    // Write each candlestick entry
    for (const auto& candle : candlesticks) {
        std::cout << "Saving Candlestick: Country = " << candle.country
                  << ", Date = " << candle.date
                  << ", Open = " << candle.open
                  << ", High = " << candle.high
                  << ", Low = " << candle.low
                  << ", Close = " << candle.close << "\n";

        file << candle.country << ","
             << candle.date << ","
             << candle.open << ","
             << candle.high << ","
             << candle.low << ","
             << candle.close << "\n";
    }

    file.close();
}


