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
        size_t shift = stop.find(':'); //находим первое ":" 
        std::string_view name = stop.substr(0, shift); //записываем слово 
        stop.remove_prefix(shift + 2); //сдвигаемся на два знака
        std::vector<std::string_view> splitted = SplitIntoWords(stop, ", "); //определяем строку до ","
        std::vector<std::pair<std::string, size_t>> stops_distance; //объявляем вектор пар
        transport_catalogue::Coordinates coordinates{ std::stod(std::string(splitted[0])), //находим первую координату
                                                      std::stod(std::string(splitted[1])) };//находим вторую координату
        if (splitted.size() > 2) { // если в запросе указано больше двух остановок, добавляем расстояния между ними
            for (auto i = 2; i < splitted.size(); ++i) {
                stops_distance.push_back(ParseDistanceBetweenStops(splitted[i]));
            }
        }
        return make_tuple(name, coordinates, stops_distance); // возвращаем имя, координату, расстояние
    }

    // формируем маршруты
    std::tuple<std::string_view, std::vector<std::string_view>, bool> ProcessRouteRequest(std::string_view route) {
        size_t shift = route.find(':'); 
        std::string_view name = route.substr(0, shift);
        route.remove_prefix(shift + 2);
        std::vector<std::string_view> stops;
        bool is_circular = false;
        if (route.find('>') != route.npos) { // если маршрут круговой
            is_circular = true;
            stops = SplitIntoWords(route, " > ");
        }
        else {
            stops = SplitIntoWords(route, " - "); // если маршрут не круговой
        }
        return make_tuple(name, stops, is_circular); //возвращаем данные маршрута: имя, остановки, круговой/не круговой
    }


    //разбиваем строку на пару: остановка - расстояние
    std::pair<std::string, size_t> ParseDistanceBetweenStops(std::string_view stops_distance) {
        size_t shift = stops_distance.find('m'); // находим 'm'
        size_t distance = std::stoul(std::string(stops_distance.substr(0, shift))); //преобразуем строк до 'm' в чмсло 
        stops_distance.remove_prefix(shift + std::size("m to")); //сдвигаемся на ("m to")
        std::string stop_name = std::string(stops_distance);  
        return { stop_name, distance }; //возвращаем осановка - расстояние
    }
        
    // формирует буфер запросов: считываем количество строк равное введенному значению
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
        for (std::string_view str : buffer) {   //первый проход
            size_t shift = str.find(' ');       //ищем первый провел, что бы удалить слово Stop
            std::string_view request = str.substr(0, shift); //присваиваем слово
            str.remove_prefix(shift + 1);   //удаляем слово Stop, что-бы осталось только название остановки

            if (request == "Stop"s) { //если слово Стоп
                auto [name, coordinates, stops_distance] = ProcessStopRequest(str); //формируем данные остановки: имя, координата, расстояние
                catalogue.AddStop(name, coordinates); // добавляем имя и координату
            }
        }
        
        for (std::string_view str : buffer) { //начинаем второй проход, std::unordered_map<std::pair<const Stop *, const Stop *>, double, detail::DistanceBetweenStopsHasher> distance_between_stop_; уже заполнен всеми остановками
            size_t shift = str.find(' ');       
            std::string_view request = str.substr(0, shift);    
            str.remove_prefix(shift + 1);   
            if (request == "Stop"s) {
                auto [name, coordinates, stops_distance] = ProcessStopRequest(str);
                 auto from = catalogue.FindStop(name); // присваиваем имя первой остановки
                for (const auto& [to, distance] : stops_distance) {     //вынимаем расстояние и название второй остановки
                    catalogue.SetDistanceBetweenStops(from, catalogue.FindStop(to), distance); // записываем (перезаписываем) в distance_between_stop_ (так как хешер считает по названию остановок)
                }
            }
            else if (request == "Bus"s) {                                   //если "Bus"
                auto [name, stops, is_circular] = ProcessRouteRequest(str); //формируем маршрут
                catalogue.AddRoute(name, stops, is_circular);               //добавляем данные маршрута в каталог: имя, список остановок, тип маршрута
            }
        }
    }
}
