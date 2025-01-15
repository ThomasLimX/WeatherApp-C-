#pragma once
#include <vector>
#include "Candlestick.h"

class Visual {
public:
    void renderCandlestickChart(const std::vector<Candlestick>& data);
    void renderPaginatedCandlestickChart(const std::vector<Candlestick>& data, int itemsPerPage); //Extra page method
   
};
