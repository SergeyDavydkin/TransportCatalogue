#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <tuple>

#include "transport_catalogue.h"
#include "geo.h"

namespace transport_catalogue::input {

	// считываем строку
	std::string ReadLine();

	// считываем первое число
	int ReadLineWithNumber();

	// разбиваю строку на слова
	std::vector<std::string_view> SplitIntoWords(std::string_view text, std::string_view separator);

	//формируем коордианаты, имя остановки и расстояние
	std::tuple<std::string_view,
		transport_catalogue::Coordinates,
		std::vector<std::pair<std::string, size_t>>> ProcessStopRequest(std::string_view stop);

	// формирую маршруты
	std::tuple<std::string_view, std::vector<std::string_view>, bool> ProcessRouteRequest(std::string_view route);

	//формируем из строки на пару: остановка - расстояние
	std::pair<std::string, size_t> ParseDistanceBetweenStops(std::string_view);

	// формирует буфер запросов
	std::vector<std::string> FillInputBuffer(std::istream&);

	// выполняем запросы из буфера
	void ProcessRequest(std::vector<std::string>& buffer, transport_catalogue::TransportCatalogue& catalogue);
}
