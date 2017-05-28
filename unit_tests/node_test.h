#include "node.h"
#include <cassert>
#include <string>
#include <list>
#include <iostream>
#include <datatypes/float64.h>
#include <datatypes/int64.h>
#include <datatypes/utfstring.h>

namespace unit_tests {

void node_create_test() {
    auto new_node = Node<data::UTFString>::create(data::UTFString{"lalala"});
    assert(new_node->internal_data.content == data::UTFString{"lalala"}.content);
}

void node_modify_test() {
    auto new_node = Node<data::Int64>::create(data::Int64{20});
    assert(new_node->internal_data.content == 20);
    new_node->internal_data = 50;
    assert(new_node->internal_data.content == 50);
}

void node_link_test() {
    //Create two nodes
    auto new_node = Node<data::Float64>::create(data::Float64{0.64});
    auto related_node = Node<data::Float64>::create(data::Float64{0.46});
    auto related_node_two = Node<data::Float64>::create(data::Float64{11.35});

    //link the second to the first
    new_node->links = std::vector<std::shared_ptr<Node<data::Float64>>>{related_node, related_node_two};

    //Make sure the second node has the right amount of kids
    assert(new_node->links.size() == 2);
    //add second to links

    new_node->links.push_back(related_node_two);

    //Make sure the second node has the right amount of kids
    assert(new_node->links.size() == 3);

    //Get a shared pointer from the first to the second
    auto related_node_sp = related_node;

    //Assert data is shared between the two, not copied
    assert(&(related_node_sp->internal_data.content)== &(related_node->internal_data.content));
    //Assert data is the same
    assert(related_node_sp->internal_data.content == related_node->internal_data.content);
    //Assert reference count is correct
    assert(related_node.use_count() == related_node_sp.use_count());

    assert(related_node.use_count() == 3);
    assert(related_node_two.use_count() == 3);
    assert(new_node.use_count() == 1);
}

void node_serialization_test() {
    auto new_node = Node<data::UTFString>::create(data::UTFString{"4325235"});
    assert(new_node->serialize_node().first == "0||7");
}

void run_node_unit_tests_suite() {
    node_create_test();
    std::cout << "Node creation unit tests suite passed\n";
    node_modify_test();
    std::cout << "Node modification unit tests suite passed\n";
    node_link_test();
    std::cout << "Node linking unit tests suite passed\n";
    node_serialization_test();
    std::cout << "Node serialization unit tests suite passed\n";
}
}
