#pragma once
#include <string>
#include <vector>

#include "geo.h"

/*
 * В этом файле вы можете разместить классы/структуры, которые являются частью предметной области (domain)
 * вашего приложения и не зависят от транспортного справочника. Например Автобусные маршруты и Остановки.
 *
 * Их можно было бы разместить и в transport_catalogue.h, однако вынесение их в отдельный
 * заголовочный файл может оказаться полезным, когда дело дойдёт до визуализации карты маршрутов:
 * визуализатор карты (map_renderer) можно будет сделать независящим от транспортного справочника.
 *
 * Если структура вашего приложения не позволяет так сделать, просто оставьте этот файл пустым.
 *
 */

namespace domain {

	struct Stop
	{		
		Stop(const std::string& n, const geo::Coordinates& coord);
			
		std::string name;
		geo::Coordinates coordinates;
	};

	using StopPtr = const Stop*;

	struct Bus
	{
		std::string name;
		std::vector<const Stop*> stops;
		bool is_circular;
	};

	using BusPtr = const Bus*;

	struct RouteInfo
	{
		std::string_view name;
		int stops = 0;
		int unique_stops = 0;
		double route_length = 0;
		double curvature = 0.0;
	};

} // namespace Domain