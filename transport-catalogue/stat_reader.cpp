#include "stat_reader.h"

using namespace std::literals;

namespace transport_catalogue::output {

    void ProcessStatRequests(std::vector<std::string>& buffer,
        transport_catalogue::TransportCatalogue& catalogue,
        std::ostream& out) {
        for (std::string_view str : buffer) {
            if (str.substr(0, "Bus"s.size()) == "Bus"s) {
                str.remove_prefix("Bus "s.size());
                if (catalogue.FindRoute(str) == nullptr) {
                    PrintEmpty("Bus"s, str, out);
                }
                else {
                    auto route_info = catalogue.GetRouteInfo(str);
                    PrintRouteInfo(route_info, out);
                }
            }
            else if (str.substr(0, "Stop"s.size()) == "Stop"s) {
                str.remove_prefix("Stop "s.size());
                if (catalogue.FindStop(str) == nullptr) {
                    PrintEmpty("Stop"s, str, out);
                }
                else {
                    PrintStopInfo(str, catalogue, out);
                }
            }
        }
    }

    void PrintRouteInfo(RouteInfo& route, std::ostream& out) {
        out << "Bus "s << route.name << ": "s
            << route.stops << " stops on route, "s
            << route.unique_stops << " unique stops, "s
            << route.route_length << " route length, "s
            << route.curvature << " curvature"s << std::endl;
    }

    void PrintStopInfo(std::string_view name, transport_catalogue::TransportCatalogue& catalogue, std::ostream& out) {
        const std::set<std::string>& stops = catalogue.GetStopInfo(name);
        out << "Stop "s << name << ":"s;
        if (stops.empty()) {
            out << " no buses"s << std::endl;
        }
        else {
            out << " buses"s;
            for (auto& stop : stops) {
                out << ' ' << stop;
            }
            out << std::endl;
        }
    }

    void PrintEmpty(std::string_view type, std::string_view name, std::ostream& my_output) {
        my_output << type << ' ' << name << ": "s << "not found"s << std::endl;
    }
}
