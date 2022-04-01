#pragma once
#include "transport_catalogue.h"
#include "geo.h"
#include <string>
#include <vector>
#include <iostream>

namespace transport_catalogue::output {

    //формирование буфера данных вывода
    void ProcessStatRequests(std::vector<std::string>& buffer,
        TransportCatalogue& catalogue,
        std::ostream& out = std::cout);

    // вывод информации о маршруте
    void PrintRouteInfo(RouteInfo& route, std::ostream& out);

    // вывод информации об остановках
    void PrintStopInfo(std::string_view name, TransportCatalogue& catalogue, std::ostream& out);

    //вывод отсутствия маршрута/остановок
    void PrintEmpty(std::string_view type, std::string_view name, std::ostream& out);
}
