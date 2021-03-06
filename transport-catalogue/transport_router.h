#pragma once

#include "domain.h"
#include "geo.h"
#include "svg.h"
#include "router.h"
#include "transport_catalogue.h"

#include <algorithm>
#include <map>
#include <memory>


namespace transport_catalogue {
    struct RouterSettings {      
        double waiting_time = 0.0;      
        double travel_time = 0.0;
    };
} // namespace transport_catalogue

namespace transport_router {

    using namespace transport_catalogue;
    
    enum class edge_type {
        WAIT,
        BUS
    };

    struct Edges {
        edge_type type;
        std::string_view name;      
        double time;
        size_t span_count;
    };

    enum class vertex_type {
        IN,
        OUT,
        EMPTY
    };

    struct Vertex {
        std::string_view name;
        vertex_type type = vertex_type::EMPTY;
        size_t id;
    };

    struct StopAsVertexes {
        Vertex in;
        Vertex out;
    };
    
    class TransportRouter {
    public:
        TransportRouter(const transport_catalogue::TransportCatalogue& catalogue)
            :catalogue_(catalogue)
        {}

        void SetSettings(const RouterSettings& settings);
        RouterSettings GetSettings() const;

        using RouteData = graph::Router<double>::RouteInfo;
        void GenerateRouter();
        void GenerateEmptyRouter(); 
        std::optional<RouteData> GetRoute(std::string_view from, std::string_view to);
        std::unique_ptr<graph::Router<double>>& ModifyRouter(); 
        const graph::Router<double>::RoutesInternalData& GetRouterData() const; 

        const graph::DirectedWeightedGraph<double>& GetGraph() const; 
        graph::DirectedWeightedGraph<double>& ModifyGraph(); 

        const std::vector<Edges>* GetEdgesData() const;
        std::vector<Edges>& ModifyEdgesData();

        const std::map<std::string_view, StopAsVertexes>* GetVertexes() const; 
        std::map<std::string_view, StopAsVertexes>& ModifyVertexes(); 
    private:
        RouterSettings settings_;
        const transport_catalogue::TransportCatalogue& catalogue_;
        std::unique_ptr<graph::Router<double>> router_ = nullptr;
        graph::DirectedWeightedGraph<double> graph_;
        std::vector<Edges> edges_;
        std::map<std::string_view, StopAsVertexes> vertexes_;
         
        void AddStops();
        double CalculateWeight(int distance);
        void AddEdges();
    };
} // namespace map_renderer
