#include "visual.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"

void Visual::renderCandlestickChart(const std::vector<Candlestick>& data) {
    if (data.empty()) {
        std::cout << "No data to visualize.\n";
        return;
    }

    // Determine temperature range
    double maxTemp = -1e9, minTemp = 1e9;
    for (const auto& candle : data) {
        maxTemp = std::max({maxTemp, candle.high});
        minTemp = std::min({minTemp, candle.low});
    }

    // Dynamically calculate chart height based on the temperature range
    int terminalHeight = 50;  // Adjust this value for your terminal size
    int chartHeight = std::min(terminalHeight, static_cast<int>(maxTemp - minTemp + 1));
    double scale = (maxTemp - minTemp) / chartHeight;

    // Adjust horizontal spacing dynamically based on dataset size
    int horizontalSpacing = std::max(1, 150 / static_cast<int>(data.size()));

    // Print the chart grid
    for (int row = chartHeight; row >= 0; --row) {
        double currentTemp = minTemp + row * scale;

        // Print Y-axis label
        std::cout << std::setw(6) << std::fixed << std::setprecision(1) << currentTemp << " | ";

        // Print candlesticks
        for (const auto& candle : data) {
            if (std::abs(currentTemp - candle.high) < scale / 2.0) {
                std::cout << RED << "=" << RESET;  // High marker
            } else if (std::abs(currentTemp - candle.low) < scale / 2.0) {
                std::cout << CYAN << "=" << RESET;  // Low marker
            } else if (currentTemp <= std::max(candle.open, candle.close) &&
                       currentTemp >= std::min(candle.open, candle.close)) {
                // Body between open and close
                if (candle.close > candle.open) {
                    std::cout << "\033[42m \033[0m";  // Bullish
                } else {
                    std::cout << "\033[41m \033[0m";  // Bearish
                }
            } else if (currentTemp <= candle.high && currentTemp >= candle.low) {
                std::cout << YELLOW << "|" << RESET;  // Stalk
            } else {
                std::cout << ".";  // Empty space
            }

            // Add spacing between candlesticks
            for (int i = 0; i < horizontalSpacing; ++i) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }

    // Print the X-axis labels vertically
    std::cout << "       "; // Align with Y-axis labels
    for (const auto& candle : data) {
        std::string year = candle.date.substr(2, 2); // Extract last two digits of the year
        std::cout << std::setw(horizontalSpacing + 1) << year;
    }
    std::cout << "\n";

    std::cout << "       "; // Align with Y-axis labels
    for (const auto& candle : data) {
        std::string month = candle.date.substr(5, 2); // Extract month
        std::cout << std::setw(horizontalSpacing + 1) << month;
    }
    std::cout << "\n";
}

void Visual::renderPaginatedCandlestickChart(const std::vector<Candlestick>& data, int itemsPerPage) {
    int totalItems = data.size();
    int pageNumber = 0;

    while (pageNumber * itemsPerPage < totalItems) {
        // Display range information for the current page
        std::cout << "The visualization continues -> ("
                  << (pageNumber * itemsPerPage + 1) << " - "
                  << std::min((pageNumber + 1) * itemsPerPage, totalItems) << ")\n";

        // Determine the subset of data for the current page
        int startIdx = pageNumber * itemsPerPage;
        int endIdx = std::min(startIdx + itemsPerPage, totalItems);

        // Debug output
        std::cout << "Rendering items " << startIdx + 1 << " to " << endIdx << " out of " << totalItems << "\n";

        // Render the selected subset of candlesticks
        renderCandlestickChart(std::vector<Candlestick>(data.begin() + startIdx, data.begin() + endIdx));

        // Add a spacer
        std::cout << "\n--- End of Page " << pageNumber + 1 << " ---\n\n";

        // Increment to the next page
        pageNumber++;
    }
}




