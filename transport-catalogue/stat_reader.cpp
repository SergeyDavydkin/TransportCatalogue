#include "stat_reader.h"

using namespace std::literals;

namespace transport_catalogue::output {

    
    void ProcessStatRequests(std::vector<std::string>& buffer,
        transport_catalogue::TransportCatalogue& catalogue,
        std::ostream& out) {
        for (std::string_view str : buffer) {               //проходим по буферу запроса
            if (str.substr(0, "Bus"s.size()) == "Bus"s) {   // если "Bus"
                str.remove_prefix("Bus "s.size());          // оставляем название автобуса/маршрута
                if (catalogue.FindRoute(str) == nullptr) {  //если каталог не содержит маршрута
                    PrintEmpty("Bus"s, str, out);           // петчатаем его отсутствие
                }
                else {                                              // если содержит маршрут 
                    auto route_info = catalogue.GetRouteInfo(str); //присваиваем информацию о маршруте
                    PrintRouteInfo(route_info, out);               // выводим  информацию о маршруте
                }
            }
            else if (str.substr(0, "Stop"s.size()) == "Stop"s) {  // аналогично для остановок
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

    void PrintRouteInfo(RouteInfo& route, std::ostream& out) { // формирование выходных данных о маршруте
        out << "Bus "s << route.name << ": "s                  // вывод название автобуса/маршрута
            << route.stops << " stops on route, "s             // вывод количества остановок
            << route.unique_stops << " unique stops, "s         // вывод количества уникальных остановок
            << route.route_length << " route length, "s         // длина маршрута
            << route.curvature << " curvature"s << std::endl;   //кривизна маршрута
    }

    //вывод остановок для запроса по автобусам
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

    //вывод отсутствия маршрутов, автобусов, остановок 
    void PrintEmpty(std::string_view type, std::string_view name, std::ostream& my_output) {
        my_output << type << ' ' << name << ": "s << "not found"s << std::endl;
    }
}
