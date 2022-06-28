 #pragma once

#include <transport_catalogue.pb.h> 

#include "transport_catalogue.h"
#include "map_renderer.h"
#include "transport_router.h"

#include <string_view>
#include <map>

namespace serialization {

	using namespace std::literals;
	using namespace transport_catalogue;

	struct SerializationSettings {
		std::string file_name = ""s;
	};

	class Serialization {
	public:
		Serialization(transport_catalogue::TransportCatalogue& catalogue, map_renderer::MapRenderer& renderer, transport_router::TransportRouter& router);
		
		void SetSettings(const SerializationSettings& settings);

	public:
		void Serialize();
		void Deserialize();

	private:
		serialization::CatalogueData SerializeCatalogueData();
		void DeserializeCatalogueData(const serialization::CatalogueData& base);

		serialization::Stop SerializeStopData(const domain::Stop& stop, const std::map<std::string_view, int>& stops_ind);
		serialization::Bus SerializeBusData(const domain::Bus& bus, const std::map<std::string_view, int>& stops_ind);
		serialization::DistanceBetweenStops SerializeDistanceData(const std::pair<std::string_view, std::string_view> stops, int length, const std::map<std::string_view, int>& stops_ind);
		
		// Cериализация MapRenderer
		serialization::MapRenderer SerializeMapRendererData();
		serialization::RenderSettings SerializeRenderSettingsData();
		serialization::Color SerializeColorData(const svg::Color& catalogue_color);

		// Десериализация MapRenderer
		void DeserializeMapRendererData(const serialization::MapRenderer& base_map_renderer);
		transport_catalogue::RenderSettings DeserializeRenderSettingsData(const serialization::RenderSettings& base_render_settings);
		svg::Color DeserializeColorData(const serialization::Color& base_color);

		// Сериализация TransportRouter
		serialization::TransportRouter SerializeTransportRouterData();
		serialization::RouterSettings SerializeRouterSettingsData();
		serialization::TransportRouterData SerializeTransportRouterClassData();
		serialization::Router SerializeRouterData();
		serialization::Graph SerializeGraphData();

		//Десериализатор TransportRouter				
		void DeserializeTransportRouterData(const serialization::TransportRouter& base_transport_router);
		transport_router::RouterSettings DeserializeTrasnportRouterSettingsData(const serialization::RouterSettings& base_router_settings);
        
		// Transport router class
		void DeserializeTransportRouterClassData(const serialization::TransportRouterData& base);
		using Vertexes = std::map<std::string_view, transport_router::StopAsVertexes>;
		Vertexes DeserializeTranspRouterVertexesData(const serialization::TransportRouterData& base_transport_router_data);
		using Edges = std::vector<transport_router::Edges>;
		Edges DeserializeTranspRouterEdgesData(const serialization::TransportRouterData& base_transport_router_data);
        
		// Graph
		void DeserializeGraphData(const serialization::Graph& base_graph_data);
		std::vector<graph::Edge<double>> DeserializeGraphEdgesData(const serialization::Graph& base_graph_data);
		std::vector<graph::IncidenceList> DeserializeGraphIncidenceListsData(const serialization::Graph& base_graph_data);
        
		// Router
		void DeserializeRouterData(const serialization::Router& base_router);
		std::optional<graph::Router<double>::RouteInternalData> DeserializeRouteInternalData(serialization::VectorRID& base);


	private:
		SerializationSettings settings_;

		transport_catalogue::TransportCatalogue& catalogue_;
		map_renderer::MapRenderer& renderer_;
		transport_router::TransportRouter& router_;

		std::map<std::string_view, int> ser_stops_ind;	
		std::map<std::string_view, int> ser_buses_ind;	

		std::map<int, std::string_view> deser_stops_ind;	
		std::map<int, std::string_view> deser_buses_ind;	
	};
}
