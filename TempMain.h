#pragma once
#include <string>
#include <vector> // For std::vector
#include <map>    // For std::map
#include "Candlestick.h" // Include the Candlestick class



class TempMain {

public:
    TempMain();
    void init();
private:
//Starting Menu
    void printMenu();
    int getUserOption();
    void processUserOption(int userOption);
//Functions
    void printHelp();
    void showCandleData();
    void printCandlesticks();
    void predictTemp();
    void filterTemperature();
    void filterDate();
    void filterRaw();
};