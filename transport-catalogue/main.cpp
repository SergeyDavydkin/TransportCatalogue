#include "json_reader.h"
#include "serialization.h"

#include <iostream>
#include <fstream>
#include <string_view>

using namespace std;
using namespace std::literals::string_literals;

using namespace std::literals;

void PrintUsage(std::ostream& stream = std::cerr) {
    stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        PrintUsage();
        return 1;
    }

    const std::string_view mode(argv[1]);

    if (mode == "make_base"sv) {
        // make base here
        transport_catalogue::TransportCatalogue catalog;
    map_renderer::MapRenderer renderer;
    transport_router::TransportRouter router(catalog);
    serialization::Serialization serialization(catalog, renderer, router);
    json_reader::JsonReader json(catalog, renderer, router, serialization);

    transport_catalogue::RequestHandler request_handler(catalog, renderer, router, serialization);
    json.ReadInput(std::cin);
    json.FillCatalogue();
    router.GenerateRouter();
    request_handler.SerializeBase();
    }
    else if (mode == "process_requests"sv) {
        // process requests here
        transport_catalogue::TransportCatalogue catalog;
    map_renderer::MapRenderer renderer;
    transport_router::TransportRouter router(catalog);
    serialization::Serialization serialization(catalog, renderer, router);
    json_reader::JsonReader json(catalog, renderer, router, serialization);

    transport_catalogue::RequestHandler request_handler(catalog, renderer, router, serialization);
    json.ReadInput(std::cin);
    serialization.Deserialize();
    json.PrintRequests(std::cout, request_handler);
    }
    else {
        PrintUsage();
        return 1;
    }
}
