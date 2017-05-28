#include "node.h"
#include "graph.h"

#include<string>
#include <vector>
#include <list>

#include <datatypes/utfstring.h>

namespace unit_tests {
Graph<data::UTFString> graph{};

void test_graph_add() {
    graph.add_entry_node(data::UTFString{"data1"});
    std::vector<data::UTFString> data_container_1{data::UTFString{"data2"},data::UTFString{"data3"}};
    graph.add_entry_nodes(data_container_1);
    std::list<data::UTFString> data_container_2{data::UTFString{"data4"},data::UTFString{"data5"}};
    graph.add_entry_nodes(data_container_2);
    std::cout << "Adding to graphs works just fine " << "\n";
}

void test_graph_query() {
    std::string match_against = "";
    auto functor = [&match_against](data::UTFString data) -> bool {
        if(data.content.find(match_against)!= std::string::npos) {
            return true;
        }
        return false;
    };

    match_against = "data";
    auto node_sp = graph.get_first_where(functor);
    assert(node_sp != nullptr);

    match_against= "brata";
    node_sp = graph.get_first_where(functor);
    assert(node_sp == nullptr);

    match_against = "data";
    auto node_container =graph.get_all_where<std::vector>(functor);
    assert(node_container.size() == 5);

    match_against = "3";
    node_container = graph.get_all_where<std::vector>(functor);
    assert(node_container.size() == 1);

    match_against= "nata";
    node_container = graph.get_all_where<std::vector>(functor);
    assert(node_container.size() == 0);

    std::cout << "Graph filter queries unit tests passed\n";
}

void run_graph_unit_tests_suite() {
    test_graph_add();
    test_graph_query();
}
}
