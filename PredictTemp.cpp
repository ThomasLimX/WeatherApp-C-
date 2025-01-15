#include "PredictTemp.h"
#include <numeric>
#include <stdexcept>
#include <cmath>

std::vector<double> PredictTemp::predictNextMonthsLinearRegression(const std::vector<double>& temperatures, int monthsToPredict) {
    if (temperatures.size() < 2) {
        throw std::runtime_error("Not enough data points for linear regression.");
    }

    size_t n = temperatures.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (size_t i = 0; i < n; ++i) {
        double weight = 1 + static_cast<double>(i) / n; // Weight: recent months get higher weights
        sumX += i * weight;
        sumY += temperatures[i] * weight;
        sumXY += i * temperatures[i] * weight;
        sumX2 += i * i * weight;
    }

    double slope = (sumXY * n - sumX * sumY) / (sumX2 * n - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;

    std::vector<double> predictions;
    for (int i = 0; i < monthsToPredict; ++i) {
        double futureX = n + i;
        double seasonalAdjustment = 2.0 * sin(futureX * 2 * M_PI / 12); // Example: Seasonal fluctuation
        predictions.push_back(slope * futureX + intercept + seasonalAdjustment);
    }

    return predictions;
}


double PredictTemp::calculateAvgRange(const std::vector<double>& highLowDifferences) {
    if (highLowDifferences.empty()) return 5.0; // Fallback range
    double sum = std::accumulate(highLowDifferences.begin(), highLowDifferences.end(), 0.0);
    return sum / highLowDifferences.size();
}

std::vector<Candlestick> PredictTemp::generatePredictedCandlesticks(
    const std::vector<double>& predictions,
    const std::string& startMonth,
    const std::string& country
) {
    std::vector<Candlestick> predictedCandlesticks;
    std::string currentMonth = startMonth;

    double prevClose = predictions.front();
    std::vector<double> historicalHighLow = { /* Add historical high-low differences */ };
    double avgRange = calculateAvgRange(historicalHighLow);

    for (const auto& predictedTemp : predictions) {
        double high = predictedTemp + avgRange / 2;
        double low = predictedTemp - avgRange / 2;
        double open = prevClose;
        double close = predictedTemp;

        predictedCandlesticks.emplace_back(country, currentMonth, open, high, low, close);
        prevClose = close;

        // Increment month
        int year = std::stoi(currentMonth.substr(0, 4));
        int month = std::stoi(currentMonth.substr(5, 2)) + 1;
        if (month > 12) { month = 1; year++; }
        currentMonth = std::to_string(year) + "-" + (month < 10 ? "0" : "") + std::to_string(month);
    }

    return predictedCandlesticks;
}
