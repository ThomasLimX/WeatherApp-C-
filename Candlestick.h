#pragma once
#include <vector>
#include <map>
#include <string>

// Forward declaration of TempEntry
class TempEntry;

class Candlestick {
public:
    std::string country;
    std::string date;
    double open;
    double high;
    double low;
    double close;

    // Default Constructor
    Candlestick();

    // Parameterized Constructor
    Candlestick(const std::string& country, const std::string& date, double open, double high, double low, double close);

    // Function to compute candlesticks
    std::vector<Candlestick> computeCandlesticks(const std::vector<TempEntry>& entries, const std::string& filterCountry);
    
    // New read CSV
    std::vector<Candlestick> readCSV(const std::string& csvFile);

    // Old Read CSV
    std::vector<TempEntry> readOldCSV(const std::string& csvFile);


   




    
    //Compute all the candlestick data
    std::vector<Candlestick> computeAllCandlesticks(const std::vector<TempEntry>& entries);
    //Save to CSV 
     void saveToCSV(const std::vector<Candlestick>& candlesticks, const std::string& outputFile);



private:
    std::vector<std::string> tokenise(const std::string& csvLine, char separator);
    std::map<std::string, std::vector<double>> groupByYear(const std::vector<TempEntry>& entries);
};
