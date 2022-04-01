#include "input_reader.h"

using namespace std::literals;

namespace transport_catalogue::input {

    // считываем строку запроса
    std::string ReadLine() {
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    // считываем первое число
    int ReadLineWithNumber() {
        int result;
        std::cin >> result;
        ReadLine();
        return result;
    }

    // // разбиваю строку на слова
    std::vector<std::string_view> SplitIntoWords(std::string_view text, std::string_view separator) {
        std::string_view str = text;
        std::vector<std::string_view> words;
        while (true) {
            size_t space = str.find(separator);
            words.push_back(str.substr(0, space));
            if (space == str.npos) {
                break;
            }
            else {
                str.remove_prefix(space + separator.size());
            }
        }
        return words;
    }

    //формируем коордианаты, имя остановки и расстояние
    std::tuple < std::string_view,
        transport_catalogue::Coordinates,
        std::vector<std::pair<std::string, size_t>> > ProcessStopRequest(std::string_view stop) {
        size_t shift = stop.find(':');
        std::string_view name = stop.substr(0, shift);
        stop.remove_prefix(shift + 2);
        std::vector<std::string_view> splitted = SplitIntoWords(stop, ", ");
        std::vector<std::pair<std::string, size_t>> stops_distance;
        transport_catalogue::Coordinates coordinates{ std::stod(std::string(splitted[0])),
                                                      std::stod(std::string(splitted[1])) };
        if (splitted.size() > 2) {
            for (auto i = 2; i < splitted.size(); ++i) {
                stops_distance.push_back(ParseDistanceBetweenStops(splitted[i]));
            }
        }
        return make_tuple(name, coordinates, stops_distance);
    }

    // формируем маршруты
    std::tuple<std::string_view, std::vector<std::string_view>, bool> ProcessRouteRequest(std::string_view route) {
        size_t shift = route.find(':');
        std::string_view name = route.substr(0, shift);
        route.remove_prefix(shift + 2);
        std::vector<std::string_view> stops;
        bool is_circular = false;
        if (route.find('>') != route.npos) {
            is_circular = true;
            stops = SplitIntoWords(route, " > ");
        }
        else {
            stops = SplitIntoWords(route, " - ");
        }
        return make_tuple(name, stops, is_circular);
    }


    //разбиваем строку на пару: остановка - расстояние
    std::pair<std::string, size_t> ParseDistanceBetweenStops(std::string_view stops_distance) {
        size_t shift = stops_distance.find('m');
        size_t distance = std::stoul(std::string(stops_distance.substr(0, shift)));
        stops_distance.remove_prefix(shift + std::size("m to"));
        std::string stop_name = std::string(stops_distance);
        return { stop_name, distance };
    }

    // формирует буфер запросов
    std::vector<std::string> FillInputBuffer(std::istream&) {
        int request_count = ReadLineWithNumber();
        std::vector<std::string> requests;
        while (request_count) {
            requests.push_back(ReadLine());
            --request_count;
        }
        return requests;
    }

    // выполняем запросы из буфера
    void ProcessRequest(std::vector<std::string>& buffer, TransportCatalogue& catalogue) {       
        for (std::string_view str : buffer) {
            size_t shift = str.find(' ');
            std::string_view request = str.substr(0, shift);
            str.remove_prefix(shift + 1);

            if (request == "Stop"s) {
                auto [name, coordinates, stops_distance] = ProcessStopRequest(str);
                catalogue.AddStop(name, coordinates);
            }
        }
        
        for (std::string_view str : buffer) {
            size_t shift = str.find(' ');
            std::string_view request = str.substr(0, shift);
            str.remove_prefix(shift + 1);
            if (request == "Stop"s) {
                auto [name, coordinates, stops_distance] = ProcessStopRequest(str);
                 auto from = catalogue.FindStop(name);
                for (const auto& [to, distance] : stops_distance) {
                    catalogue.SetDistanceBetweenStops(from, catalogue.FindStop(to), distance);
                }
            }
            else if (request == "Bus"s) {
                auto [name, stops, is_circular] = ProcessRouteRequest(str);
                catalogue.AddRoute(name, stops, is_circular);
            }
        }
    }
}
