#include "node.h"
#include <cassert>
#include <string>
#include <list>
#include <iostream>

namespace unit_tests {

void node_create_test() {
    auto new_node = Node<std::string>::create(new std::string{"lalala"});
    assert(*new_node->internal_data.load() == std::string{"lalala"});
}

void node_modify_test() {
    auto new_node = Node<int>::create(new int{20});
    assert(*new_node->internal_data == 20);
    new_node->modify_data(new int{50});
    assert(*new_node->internal_data == 50);
}

void node_link_test() {
    //Create two nodes
    auto new_node = Node<double>::create(new double{0.64});
    auto related_node = Node<double>::create(new double{0.46});
    auto related_node_two = Node<double>::create(new double{11.35});
    
    //link the second to the first
    new_node->modify_links(new std::list<std::shared_ptr<Node<double>>>{related_node, related_node_two});
    
    //Make sure the second node has the right amount of members in its list of links
    assert(new_node->links.load()->size() == 2);
    
    //Get a shared pointer from the first to the second
    auto related_node_sp = new_node->links.load()->front();
    
    //Assert data is shared between the two, not copied
    assert(related_node_sp->internal_data == related_node->internal_data);
    //Assert data is the same
    assert(*related_node_sp->internal_data == *related_node->internal_data);
    //Assert reference count is correct
    assert(related_node.use_count() == related_node_sp.use_count());
    assert(related_node.use_count() == 3);
    assert(related_node_two.use_count() == 2);
    assert(new_node.use_count() == 1);
}

void node_serialization_test() {
    auto new_node = Node<std::string>::create(new std::string{"4325235"});
    std::cout << new_node->serialize_node_metadata();
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
