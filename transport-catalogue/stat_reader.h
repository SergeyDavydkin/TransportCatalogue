#pragma once
#include "transport_catalogue.h"
#include "geo.h"
#include <string>
#include <vector>
#include <iostream>

namespace transport_catalogue::output {

    void ProcessStatRequests(std::vector<std::string>& buffer,
        TransportCatalogue& catalogue,
        std::ostream& out = std::cout);

    void PrintRouteInfo(RouteInfo& route, std::ostream& out);

    void PrintStopInfo(std::string_view name, TransportCatalogue& catalogue, std::ostream& out);

    void PrintEmpty(std::string_view type, std::string_view name, std::ostream& out);
}
