#include "transport_router.h"

#include <math.h>

namespace transport_router {
   
    void TransportRouter::SetSettings(const RouterSettings& settings) {
        settings_ = settings;
    }

    RouterSettings TransportRouter::GetSettings() const {
        return settings_;
    }

    void TransportRouter::GenerateRouter() {
        if (router_ != nullptr) {
            router_.release();
        }
        AddStops();
        AddEdges();
        router_ = std::make_unique<graph::Router<double>>(graph::Router(graph_));
    }

    void TransportRouter::GenerateEmptyRouter() { 
        if (router_ != nullptr) { 
            router_.release();
        }
        
        router_ = std::make_unique<graph::Router<double>>(graph::Router(graph_));
    }

    std::optional<TransportRouter::RouteData> TransportRouter::GetRoute(std::string_view from, std::string_view to) {
        return router_->BuildRoute(vertexes_.at(from).in.id, vertexes_.at(to).in.id);
    }

    std::unique_ptr<graph::Router<double>>& TransportRouter::ModifyRouter() { 
        return router_;
    }

    const graph::Router<double>::RoutesInternalData& TransportRouter::GetRouterData() const { 
        return router_.get()->GetRoutesInternalData();
    }

    const graph::DirectedWeightedGraph<double>& TransportRouter::GetGraph() const { 
        return graph_;
    }

    graph::DirectedWeightedGraph<double>& TransportRouter::ModifyGraph() { 
        return graph_;
    }

    const std::vector<Edges>* TransportRouter::GetEdgesData() const {
        return &edges_;
    }

    std::vector<Edges>& TransportRouter::ModifyEdgesData() {
        return edges_;
    }

    const std::map<std::string_view, StopAsVertexes>* TransportRouter::GetVertexes() const { 
        return &vertexes_;
    }

    std::map<std::string_view, StopAsVertexes>& TransportRouter::ModifyVertexes() { 
        return vertexes_;
    }
         
    void TransportRouter::AddStops() { 
        size_t vertex_count = 0;
        for (auto [name, stop_ptr] : catalogue_.GetStopsForRender()) {
            Vertex in = { name, vertex_type::IN, vertex_count++ };
            Vertex out = { name, vertex_type::OUT, vertex_count++ };
            vertexes_[name] = { in, out };
        }
        graph_.ResizeIncidenceLists(vertexes_.size());                                                        
        for (auto [name, data] : vertexes_) {
            graph_.AddEdge({ data.in.id, data.out.id, settings_.waiting_time });
            edges_.push_back({ edge_type::WAIT, name, settings_.waiting_time, 0 }); 
        }
    }

    double TransportRouter::CalculateWeight(int distance) {
        return distance / (settings_.travel_time * 1000.0 / 60.0); 
    }

    void TransportRouter::AddEdges() {
        for (auto [name, route_ptr] : catalogue_.GetRoutesForRender()) { 
            for (auto it = route_ptr.stops.begin(); it != prev(route_ptr.stops.end()); ++it) { 
                int dist_to_next_stop = 0;
                std::string_view out_stop = *it;
                for (auto sub_it = it + 1; sub_it != route_ptr.stops.end(); ++sub_it) { 
                    if (!route_ptr.is_round_trip && 
                        sub_it - route_ptr.stops.begin() == ceil(route_ptr.stops.size() / 2) + 1 && 
                        *prev(sub_it) == route_ptr.end_stop &&
                        it - route_ptr.stops.begin() != ceil(route_ptr.stops.size() / 2)) {
                        break;
                    }
                    if (catalogue_.GetDistForRouter().count(std::pair(out_stop, *sub_it))) { 
                        dist_to_next_stop += catalogue_.GetDistForRouter().at(std::pair(out_stop, *sub_it));
                    }
                    else {
                        dist_to_next_stop += catalogue_.GetDistForRouter().at(std::pair(*sub_it, out_stop));
                    }
                    size_t span = sub_it - it; 
                    edges_.push_back({ edge_type::BUS, name, CalculateWeight(dist_to_next_stop), span });
                    graph_.AddEdge({ vertexes_.at(*it).out.id, vertexes_.at(*sub_it).in.id, edges_.back().time });
                    out_stop = *sub_it;
                }
            }
        }
    }
} // namespace transport_router
