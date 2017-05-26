#include "node.h"
#include "graph.h"

#include <vector>
#include <list>
#include <datatypes/utfstring.h>

namespace unit_tests {
Graph<data::UTFString> graph{};

void test_graph_add() {
    graph.add_entry_node(new data::UTFString{"data1"});
    std::vector<data::UTFString*> data_container_1{new data::UTFString{"data2"},new data::UTFString{"data3"}};
    graph.add_entry_nodes(data_container_1);
    std::list<data::UTFString*> data_container_2{new data::UTFString{"data4"},new data::UTFString{"data5"}};
    graph.add_entry_nodes(data_container_2);
    std::cout << "Adding to graphs works just fine " << "\n";
}

void run_graph_unit_tests_suite() {
    test_graph_add();
}
}
