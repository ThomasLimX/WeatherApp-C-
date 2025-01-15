
#include <iostream>
#include <vector>
#include "TempMain.h"
#include "TempEntry.h"
#include <iomanip>
#include "Candlestick.h"
#include "FilterData.h"
#include "Visual.h"
#include <limits>
#include <ios>
#include <cstdlib>
#include "PredictTemp.h"



TempMain::TempMain() {}

int TempMain::getUserOption(){
    int userOption;
    std::cout<<"Type in 1-7" << std::endl;
    std::cin >> userOption;
    return userOption;
}


void TempMain::init(){
while(true) {
    int input;
    printMenu();
    input = getUserOption();
    if(input == 7) break;
    processUserOption(input);
}
 
}

// Menu
void TempMain::printMenu(){

std::cout<<"============================================================="<< std::endl;
std::cout<<"                      WEATHER APP MENU                        "<< std::endl;
std::cout<<"============================================================= "<<std::endl;
std::cout<<"1.Print Help" <<std::endl;
std::cout<<"2.Show Example Data" <<std::endl;
std::cout<<"3.Print Example Candlestick Visualisation" <<std::endl;
std::cout<<"4.Filter Country's Date " <<std::endl;
std::cout<<"5.Filter Country's Temperature" <<std::endl;
std::cout<<"6.Predict Temperature" <<std::endl;
std::cout<<"7.Exit" << std::endl;
}

//Menu Functions 

void TempMain::processUserOption(int userOption) {
    if (userOption == 1){
         printHelp();
    }else if (userOption == 2){
        showCandleData();
    } else if (userOption == 3){
        printCandlesticks();
    } else if (userOption == 4){
        filterDate();
    }else if (userOption == 5) {
        filterTemperature();
    }else if (userOption == 6) {
        predictTemp();
    } else if (userOption == 7) {
        std::cout << "Exiting the program" << std::endl;
    } else {
        std::cout << "Invalid Choice. Please choose between 1-7" << std::endl;
    }
}

void TempMain::printHelp(){
    std::cout << "Our goal is to analyze the temperature in Europe!" << std::endl;
    std::cout << "For Data Visuals, It is - for High and Low Temperature" << std::endl;
    std::cout << "Red is for Highest and Blue is for Lowest" << std::endl;
    std::cout << "For Data Visuals, the square blocks for Open and Close Temperature" << std::endl;
    std::cout << "Green for Bullish and Red for Bearish" << std::endl;
}


void TempMain::showCandleData() {
    Candlestick candlestick;

    //Exception Handling 
    try {
        // Step 1: Attempt to read candlestick data from CSV
        std::vector<Candlestick> candlesticks;
        try {
            candlesticks = candlestick.readCSV("filtered.csv");
        } catch (const std::exception& ex) {
            std::cerr << "Warning: " << ex.what() << "\n";
        }

        // Step 2: Check if data is missing and generate it if needed
        if (candlesticks.empty()) {
            std::cout << "No data found in 'filtered.csv'. Creating the file now using raw data from 'weather_data.csv'.\n";
            filterRaw();

            // Retry reading the newly created file
            try {
                candlesticks = candlestick.readCSV("filtered.csv");
            } catch (const std::exception& ex) {
                std::cerr << "Error: Failed to read 'filtered.csv' after creation. " << ex.what() << "\n";
                return;
            }

            if (candlesticks.empty()) {
                std::cerr << "Error: No data could be retrieved after creating 'filtered.csv'.\n";
                return;
            }
        }

    

        // Step 2: Filter data by country ("AT") and date range (2016-01 to 2019-12)
        std::vector<Candlestick> filteredCandlesticks;
        for (const auto& candle : candlesticks) {
            if (candle.country == "AT" && candle.date >= "2016-01" && candle.date <= "2019-12") {
                filteredCandlesticks.push_back(candle);
            }
        }

        if (filteredCandlesticks.empty()) {
            std::cout << "No data found for country AT in the date range (2016-01 to 2019-12).\n";
            return;
        }

        // Step 3: Display the filtered candlestick data
        std::cout << "Filtered Candlestick Data for Country: AT (2016-01 to 2019-12):\n";
        for (const auto& candle : filteredCandlesticks) {
            std::cout << "Date: " << candle.date
                      << ", Open: " << candle.open
                      << ", High: " << candle.high
                      << ", Low: " << candle.low
                      << ", Close: " << candle.close << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}


void TempMain::printCandlesticks() {
    std::cout << "Visualizing candlestick data for country AT from 2016-01 to 2019-12\n";

    Candlestick candlestick;
    try {
        // Step 1: Attempt to read candlestick data from CSV
        std::vector<Candlestick> candlesticks;
        try {
            candlesticks = candlestick.readCSV("filtered.csv");
        } catch (const std::exception& ex) {
            std::cerr << "Warning: " << ex.what() << "\n";
        }

        // Step 2: Check if data is missing and generate it if needed
        if (candlesticks.empty()) {
            std::cout << "No data found in 'filtered.csv'. Creating the file now using raw data from 'weather_data.csv'.\n";
            filterRaw();

            // Retry reading the newly created file
            try {
                candlesticks = candlestick.readCSV("filtered.csv");
            } catch (const std::exception& ex) {
                std::cerr << "Error: Failed to read 'filtered.csv' after creation. " << ex.what() << "\n";
                return;
            }

            if (candlesticks.empty()) {
                std::cerr << "Error: No data could be retrieved after creating 'filtered.csv'.\n";
                return;
            }
        }

        // Filter by country ("AT") and date range (2016-01 to 2019-12)
        std::vector<Candlestick> filteredCandlesticks;
        for (const auto& candle : candlesticks) {
            if (candle.country == "AT" && candle.date >= "2016-01" && candle.date <= "2019-12") {
                filteredCandlesticks.push_back(candle);
            }
        }

        if (filteredCandlesticks.empty()) {
            std::cout << "No data found for country AT in the date range (2016-01 to 2019-12).\n";
            return;
        }

        // Visualize using ASCII art
        Visual visualizer;
        visualizer.renderCandlestickChart(filteredCandlesticks);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}


void TempMain::filterDate() {
    Candlestick candlestick;
    try {
        // Step 1: Attempt to read candlestick data from CSV
        std::vector<Candlestick> candlesticks;
        try {
            candlesticks = candlestick.readCSV("filtered.csv");
        } catch (const std::exception& ex) {
            std::cerr << "Warning: " << ex.what() << "\n";
        }

        // Step 2: Check if data is missing and generate it if needed
        if (candlesticks.empty()) {
            std::cout << "No data found in 'filtered.csv'. Creating the file now using raw data from 'weather_data.csv'.\n";
            filterRaw();

            // Retry reading the newly created file
            try {
                candlesticks = candlestick.readCSV("filtered.csv");
            } catch (const std::exception& ex) {
                std::cerr << "Error: Failed to read 'filtered.csv' after creation. " << ex.what() << "\n";
                return;
            }

            if (candlesticks.empty()) {
                std::cerr << "Error: No data could be retrieved after creating 'filtered.csv'.\n";
                return;
            }
        }

    
        // Step 2: Prompt user for country and date range
        std::string country, startDate, endDate;
        std::cout << "Enter Country Code (e.g., AT, BE, BG, CH, CZ, DE, DK, EE, ES, FI, FR, GB, GR, HR, HU, IE, IT, LT, LU, LV, NL, NO, PL, PT, RO, SE, SI, SK): ";
        std::cin >> country;
        std::cout << "Enter start date (YYYY-MM): ";
        std::cin >> startDate;
        std::cout << "Enter end date (YYYY-MM): ";
        std::cin >> endDate;

        // Step 3: Filter data by country
        std::vector<Candlestick> countryFiltered = FilterData::filterCandlesticksByCountryCode(candlesticks, country);

        if (countryFiltered.empty()) {
            std::cout << "No data found for the selected country.\n";
            return;
        }

        // Step 4: Filter data by date range
        std::vector<Candlestick> dateFiltered = FilterData::filterCandlesticksByDateRange(countryFiltered, startDate, endDate);

        if (dateFiltered.empty()) {
            std::cout << "No data found for the selected date range.\n";
            return;
        }

        // Step 5: Display filtered candlestick data
        std::cout << "Filtered Candlestick Data for Country: " << country << "\n";
        for (const auto& candle : dateFiltered) {
            std::cout << "Date: " << candle.date
                      << ", Open: " << candle.open
                      << ", High: " << candle.high
                      << ", Low: " << candle.low
                      << ", Close: " << candle.close << "\n";
        }

        // Step 6: Render visualization
        std::cout << "\n=== Filter Visualization for Date range by Years and Months ===\n";
        Visual visualizer;
        visualizer.renderPaginatedCandlestickChart(dateFiltered, 50);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}


void TempMain::filterTemperature() {
   Candlestick candlestick;
    try {
        // Step 1: Attempt to read candlestick data from CSV
        std::vector<Candlestick> candlesticks;
        try {
            candlesticks = candlestick.readCSV("filtered.csv");
        } catch (const std::exception& ex) {
            std::cerr << "Warning: " << ex.what() << "\n";
        }

        // Step 2: Check if data is missing and generate it if needed
        if (candlesticks.empty()) {
            std::cout << "No data found in 'filtered.csv'. Creating the file now using raw data from 'weather_data.csv'.\n";
            filterRaw();

            // Retry reading the newly created file
            try {
                candlesticks = candlestick.readCSV("filtered.csv");
            } catch (const std::exception& ex) {
                std::cerr << "Error: Failed to read 'filtered.csv' after creation. " << ex.what() << "\n";
                return;
            }

            if (candlesticks.empty()) {
                std::cerr << "Error: No data could be retrieved after creating 'filtered.csv'.\n";
                return;
            }
        }

    

        // Step 2: Prompt user for country and temperature range
        std::string country;
        double minTemperature, maxTemperature;
        std::cout << "Enter Country Code (e.g., AT, BE, BG, CH, CZ, DE, DK, EE, ES, FI, FR, GB, GR, HR, HU, IE, IT, LT, LU, LV, NL, NO, PL, PT, RO, SE, SI, SK): ";
        std::cin >> country;
        std::cout << "Enter Minimum Temperature: ";
        std::cin >> minTemperature;
        std::cout << "Enter Maximum Temperature: ";
        std::cin >> maxTemperature;

        if (minTemperature > maxTemperature) {
            std::cout << "Invalid range: Minimum temperature cannot exceed maximum temperature.\n";
            return;
        }

        // Step 3: Filter data by country
        std::vector<Candlestick> countryFiltered = FilterData::filterCandlesticksByCountryCode(candlesticks, country);

        if (countryFiltered.empty()) {
            std::cout << "No data found for the selected country.\n";
            return;
        }

        // Step 4: Filter data by temperature range
        std::vector<Candlestick> tempFiltered = FilterData::filterCandlesticksByTemperature(countryFiltered, minTemperature, maxTemperature);

        if (tempFiltered.empty()) {
            std::cout << "No data found for the given temperature range.\n";
            return;
        }

        // Step 5: Display filtered candlestick data
        std::cout << "Filtered Candlestick Data for Country: " << country << "\n";
        for (const auto& candle : tempFiltered) {
            std::cout << "Date: " << candle.date
                      << ", Open: " << candle.open
                      << ", High: " << candle.high
                      << ", Low: " << candle.low
                      << ", Close: " << candle.close << "\n";
        }

        // Step 6: Render visualization
        std::cout << "\n=== Filter Visualisation for Temperature Range ===\n";
        Visual visualizer;
        visualizer.renderPaginatedCandlestickChart(tempFiltered, 50);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}





void TempMain::predictTemp() {
    std::cout << "=== Predict Temperatures for the Timeframe using Linear Regression ===\n";

     Candlestick candlestick;
    try {
        // Step 1: Attempt to read candlestick data from CSV
        std::vector<Candlestick> candlesticks;
        try {
            candlesticks = candlestick.readCSV("filtered.csv");
        } catch (const std::exception& ex) {
            std::cerr << "Warning: " << ex.what() << "\n";
        }

        // Step 2: Check if data is missing and generate it if needed
        if (candlesticks.empty()) {
            std::cout << "No data found in 'filtered.csv'. Creating the file now using raw data from 'weather_data.csv'.\n";
            filterRaw();

            // Retry reading the newly created file
            try {
                candlesticks = candlestick.readCSV("filtered.csv");
            } catch (const std::exception& ex) {
                std::cerr << "Error: Failed to read 'filtered.csv' after creation. " << ex.what() << "\n";
                return;
            }

            if (candlesticks.empty()) {
                std::cerr << "Error: No data could be retrieved after creating 'filtered.csv'.\n";
                return;
            }
        }

    

        // Step 2: Prompt user for country and date range
        std::string country, startDate, endDate;
        std::cout << "Enter Country Code (e.g., AT, BE, BG, CH, CZ, DE, DK, EE, ES, FI, FR, GB, GR, HR, HU, IE, IT, LT, LU, LV, NL, NO, PL, PT, RO, SE, SI, SK): ";
        std::cin >> country;
        std::cout << "Enter start date (YYYY-MM): ";
        std::cin >> startDate;
        std::cout << "Enter end date (YYYY-MM): ";
        std::cin >> endDate;

        // Step 3: Filter data by country and date range
        std::vector<Candlestick> filteredCandlesticks;
        for (const auto& candle : candlesticks) {
            if (candle.country == country &&
                candle.date >= startDate &&
                candle.date <= endDate) {
                filteredCandlesticks.push_back(candle);
            }
        }

        if (filteredCandlesticks.empty()) {
            std::cout << "No data found for the selected filters.\n";
            return;
        }

        // Step 4: Extract closing temperatures
        std::vector<double> closingTemperatures;
        for (const auto& candle : filteredCandlesticks) {
            closingTemperatures.push_back(candle.close);
        }

        // Step 5: Predict the next Timeframe using Linear Regression
        std::vector<double> predictions = PredictTemp::predictNextMonthsLinearRegression(closingTemperatures, 50);

        // Step 6: Generate candlesticks for predicted data
        std::vector<Candlestick> predictedCandlesticks =
            PredictTemp::generatePredictedCandlesticks(predictions, endDate, country);

        // Step 7: Visualize historical and predicted candlesticks
        std::cout << "\n=== Historical Data Visualization ===\n";
        Visual visualizer;
        visualizer.renderPaginatedCandlestickChart(filteredCandlesticks, 50);

        std::cout << "\n=== Predicted Data Visualization ===\n";
        visualizer.renderPaginatedCandlestickChart(predictedCandlesticks, 50);

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}




//Filter all the rawData for faster processing 

void TempMain::filterRaw() {
    Candlestick candlestick;

    try {
        // Step 1: Read raw data from CSV
        std::vector<TempEntry> entries = candlestick.readOldCSV("weather_data.csv");
        if (entries.empty()) {
            std::cout << "No data found. Please check the CSV file.\n";
            return;
        }

        // Step 2: Compute candlestick data for all countries
        std::vector<Candlestick> allCandlesticks = candlestick.computeAllCandlesticks(entries);
        if (allCandlesticks.empty()) {
            std::cout << "No candlestick data could be generated.\n";
            return;
        }

        // Step 3: Save the computed data to a CSV file
        candlestick.saveToCSV(allCandlesticks, "filtered.csv");
        std::cout << "Candlestick data saved to 'filtered.csv'.\n";

        // Step 4: Display the saved data (optional)
        for (const auto& candle : allCandlesticks) {
            std::cout << "Country: " << candle.country
                      << ", Date: " << candle.date
                      << ", Open: " << candle.open
                      << ", High: " << candle.high
                      << ", Low: " << candle.low
                      << ", Close: " << candle.close << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}

