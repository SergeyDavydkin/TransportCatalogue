#include "transport_catalogue.h"

#include <iomanip>

namespace transport_catalogue {
	size_t DistanceBetweenStopsHasher::operator()(const std::pair<std::string_view, std::string_view>& stops_pair) const {
		return hasher(stops_pair.first) + 37 * hasher(stops_pair.second);
	}
    
    TransportCatalogue::TransportCatalogue() {};
    
	void TransportCatalogue::AddRoute(std::string name, const std::vector<std::string_view>& data, bool is_round, std::string_view end_stop) {
		std::string& ref_to_name = buses_names_.emplace_back(std::move(name)); 
		std::string_view sv_name{ ref_to_name }; 
		std::vector<std::string_view> v_stops;
		for (std::string_view stop : data) {
			v_stops.emplace_back(stops_[stop].name);
			stops_[stop].buses.insert(sv_name); 
		}
		routes_[sv_name] = { sv_name, v_stops, is_round, stops_.at(end_stop).name };
	}

	const domain::Bus* TransportCatalogue::SearchRoute(std::string_view name) const {
		if (!routes_.count(name)) {
			return nullptr;
		}
		return &routes_.at(name);
	}

	TransportCatalogue::RouteOutput TransportCatalogue::GetRoute(std::string_view name) {
		RouteOutput result;
		result.name = name.substr(0, name.size());
		if (!routes_.count(name)) {
			result.name.insert(0, "!");
			return result;
		}
		else {
			const domain::Bus tmp = *SearchRoute(name);
			result.real_stops_count = tmp.stops.size();			
			std::set<std::string_view> unique_stops{ tmp.stops.begin(), tmp.stops.end() };
			result.unique_stops_count = unique_stops.size();
			double road_distance = 0;
			double geo_distance = 0;
			for (auto it = tmp.stops.begin(); it < prev(tmp.stops.end()); ++it) { 
				road_distance += GetDistBtwStops((*it), *next(it));
				geo_distance += ComputeDistance(SearchStop(*it)->coordinates, SearchStop(*(it + 1))->coordinates);
			}
			result.route_length = road_distance;
			result.curvature = road_distance / geo_distance;
			return result;
		}
	}
		
	void  TransportCatalogue::AddStop(std::string name, const geo::Coordinates& coordinates) {
		std::string& ref_to_name = stops_names_.emplace_back(std::move(name)); 
		std::string_view sv_name{ ref_to_name }; 
		geo::Coordinates geo = { coordinates.lat, coordinates.lng };
		stops_[sv_name] = { sv_name, geo, {} }; 
	}

	const  domain::Stop* TransportCatalogue::SearchStop(std::string_view name) const {
		if (!stops_.count(name)) {
			return nullptr;
		}
		return &stops_.at(name);
	}

	TransportCatalogue::StopOutput  TransportCatalogue::GetStop(std::string_view name) {
		StopOutput result;
		result.name = name.substr(0, name.size());
		if (!stops_.count(name)) {
			result.name.insert(0, "!");
			return result;
		}
		else {
			const domain::Stop tmp = *SearchStop(name);
			result.buses = stops_[name].buses;
			return result;
		}
	}
	
	void  TransportCatalogue::SetDistBtwStops(std::string_view name, std::string_view name_to, const int dist) {
		const domain::Stop stop = *SearchStop(name); 
		const domain::Stop stop_to = *SearchStop(name_to);
		dist_btw_stops_[std::pair(stop.name, stop_to.name)] = dist;
	}

	int  TransportCatalogue::GetDistBtwStops(std::string_view name, std::string_view name_to) {
		if (dist_btw_stops_.count(std::pair(name, name_to))) {
			return dist_btw_stops_.at(std::pair(name, name_to));
		}
		else {			
			return dist_btw_stops_.at(std::pair(name_to, name));
		}
	}
    
   const std::unordered_map<std::string_view, domain::Bus>& TransportCatalogue::GetRoutesForRender() const { 
            return routes_; 
		} 
    
    const std::unordered_map<std::string_view, domain::Stop>& TransportCatalogue::GetStopsForRender() const {
			return stops_;
		}
    
    const std::unordered_map<std::pair<std::string_view, std::string_view>, int, DistanceBetweenStopsHasher>& TransportCatalogue::GetDistForRouter() const {
        return dist_btw_stops_;
    }
    
} // namespace transport_catalogue
