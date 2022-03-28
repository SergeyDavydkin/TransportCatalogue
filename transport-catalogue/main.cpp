#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
#include <sstream>

using namespace transport_catalogue;

int main() {
    TransportCatalogue transport_catalogue;
    std::vector<std::string> requests = input::FillInputBuffer(std::cin);
    input::ProcessRequest(requests, transport_catalogue);

    requests.clear();
    requests = input::FillInputBuffer(std::cin);
    output::ProcessStatRequests(requests, transport_catalogue);
    return 0;
}
