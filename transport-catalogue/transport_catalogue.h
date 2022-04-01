#pragma once
#include "geo.h"
#include <algorithm>
#include <deque>
#include <execution>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <string>
#include <string_view>
#include <functional>


namespace transport_catalogue {
    
    //структура остановок: конструктор, имя, координата 
    struct Stop
    {
        Stop(const std::string& n, const Coordinates& coord)
            : name(n), coordinates(coord) {
        }
        std::string name;
        Coordinates coordinates;
        
    };
    
    //структура автобусов(маршрутов): имя, остановки, круговой/не круговой 
    struct Bus
    {
        std::string name;
        std::vector<const Stop*> stops;
        bool is_circular;
    };
    
    /// информация о маршруте:имя (номера автобуса), списка остановок, уникальных остановок, длина, кривизна (длина/длина по прямой)
    struct RouteInfo
    {
        std::string_view name;
        int stops;
        int unique_stops;
        size_t route_length;
        double curvature;
    };
    
    //хеш функция для пары остановок
    struct DistanceBetweenStopsHasher {
    public:
        size_t operator() (const std::pair<const Stop*, const Stop*> stops_pair) const {
            return hasher(stops_pair.first) + 37 * hasher(stops_pair.second);
        }
    private:
        std::hash<const void*> hasher;
    };

    // TransportCatalogue основной класс транспортного каталога
    class TransportCatalogue
    {
    public:
        // формирует маршрут из списка остановок и добавляет его в каталог        
        void AddRoute(std::string_view name, const std::vector<std::string_view>& stops, bool is_circular);
        // добавляет остановку в каталог
        void AddStop(std::string_view name, const Coordinates& coordinates);
        // возвращает информацию о маршруе по его имени
        RouteInfo GetRouteInfo(std::string_view route);
        // возвращает указатель на маршрут по его имени
        const Bus* FindRoute(std::string_view route_name);
        // возвращает указатель на остановку по её имени
        const Stop* FindStop(std::string_view stop_name);
        //возвращает список автобусов, проходящих через остановку
        std::set<std::string> GetStopInfo(std::string_view stop_name);
       // добавляет в каталог информацию о расстоянии между двумя остановками
        void SetDistanceBetweenStops(const Stop* from, const Stop* to, size_t distance);
        //возвращает расстояние между остановками 1 и 2 - в прямом, либо если нет - в обратном направлении
        size_t GetDistanceBetweenStops(const Stop* from, const Stop* to);

    private:
        // автобусы(маршруты)
        std::deque<Bus> buses_;
        // остановки
        std::deque<Stop> stops_;
        // автобусы(маршруты)
        std::unordered_map<std::string_view, const Bus*> buses_name_;
        // остановки
        std::unordered_map<std::string_view, const Stop*> stops_name_;
        // автобусы на каждой остановке
        std::unordered_map<const Stop*, std::set<std::string>> buses_in_stop_;
        //расстояние между остановками
        std::unordered_map<std::pair<const Stop*, const Stop*>, double, DistanceBetweenStopsHasher> distance_between_stops_;
    };
}
