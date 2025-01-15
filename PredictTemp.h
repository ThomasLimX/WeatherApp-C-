#include <vector>
#include <string>
#include "Candlestick.h" // Ensure this is included

class PredictTemp {
public:
    // Predict the next N months using linear regression
    static std::vector<double> predictNextMonthsLinearRegression(const std::vector<double>& temperatures, int monthsToPredict);

    // Generate candlesticks for predicted temperatures
    static std::vector<Candlestick> generatePredictedCandlesticks(
        const std::vector<double>& predictions,
        const std::string& startMonth,
        const std::string& country
    );

    static double calculateAvgRange(const std::vector<double>& highLowDifferences);
};