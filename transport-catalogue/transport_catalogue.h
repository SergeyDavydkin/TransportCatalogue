#pragma once

#include "domain.h"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <deque>
#include <cassert>
#include <iostream>
#include <set>

namespace transport_catalogue {
	struct DistanceBetweenStopsHasher {
		size_t operator()(const std::pair<std::string_view, std::string_view>& stops_pair) const;
		std::hash<std::string_view> hasher;       
	};    

	class TransportCatalogue {
	public:
		TransportCatalogue();
		
		void AddRoute(std::string name, const std::vector<std::string_view>& data, bool is_round, std::string_view end_stop);       
        
		const domain::Bus* SearchRoute(std::string_view name) const;

		struct RouteOutput {
			std::string name = {}; 
			size_t real_stops_count = {};
			size_t unique_stops_count = {};
			double route_length = {};
			double curvature = {};
		};
		RouteOutput GetRoute(std::string_view name);
		
		void AddStop(std::string name, const geo::Coordinates& coordinates);
		const domain::Stop* SearchStop(std::string_view name) const;

		struct StopOutput {
			std::string name = {}; 
			std::set<std::string_view> buses = {};
		};
		StopOutput GetStop(std::string_view name);
		
		void SetDistBtwStops(std::string_view name, std::string_view name_to, const int dist);
		int GetDistBtwStops(std::string_view name, std::string_view name_to);		
        
        const std::unordered_map<std::string_view, domain::Bus>& GetRoutesForRender() const;		
        
        const std::unordered_map<std::string_view, domain::Stop>& GetStopsForRender() const;
		
        const std::unordered_map<std::pair<std::string_view, std::string_view>, int, DistanceBetweenStopsHasher>& GetDistForRouter() const;
		
		~TransportCatalogue() {};
	private:
		std::deque<std::string> stops_names_; 
		std::deque<std::string> buses_names_; 
		
		std::unordered_map<std::pair<std::string_view, std::string_view>, int, DistanceBetweenStopsHasher> dist_btw_stops_; 

		std::unordered_map<std::string_view, domain::Stop> stops_;
		std::unordered_map<std::string_view, domain::Bus> routes_;
	};
} // namespace transport_catalogue
