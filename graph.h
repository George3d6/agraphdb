#pragma once

#include<vector>
#include<memory>
#include<mutex>
#include<utility>
#include<shared_mutex>
#include<node.h>


template <class NodeData>
class Graph {
public:
    using TypedNode = Node<NodeData>;
    using NodeSP = std::shared_ptr<Node<NodeData>>;
    using NodeWP = std::weak_ptr<Node<NodeData>>;

    void add_entry_node(NodeData data) noexcept {
        auto node_sp = TypedNode::create(data);
        std::unique_lock<std::shared_mutex> wlock{entry_nodes_mutex};
        all_nodes.push_back(node_sp);
        entry_nodes.push_back(std::move(node_sp));
    }

    template <class Container>
    void add_entry_nodes(Container data_container) noexcept {
        std::unique_lock<std::shared_mutex> wlock{entry_nodes_mutex};
        for(auto data : data_container) {
            NodeSP node_sp = TypedNode::create(data);
            all_nodes.push_back(node_sp);
            entry_nodes.push_back(std::move(node_sp));
        }
    }


    template<typename Function>
    NodeSP get_first_where(Function pick) const {
       std::shared_lock<std::shared_mutex> rlock{all_nodes_mutex};
       for(const NodeWP& node_wp : all_nodes) {
            NodeSP node_sp = node_wp.lock();
            if(node_sp != nullptr) {
                if(pick(node_sp->internal_data)) {
                    return node_sp;
                }
            }
       }
       return nullptr;
    }

    template<template <typename...> class Container, typename np = NodeSP, typename Function>
    Container<np> get_all_where(Function pick) const {
       Container<np> new_nodes{};
       std::shared_lock<std::shared_mutex> rlock{this->all_nodes_mutex};
       for(const NodeWP& node_wp : all_nodes) {
            NodeSP node_sp = node_wp.lock();
            if(node_sp != nullptr) {
                if(pick(node_sp->internal_data)) {
                    new_nodes.push_back(node_sp);
                }
            }
       }
       return new_nodes;
    }



private:
    std::vector<NodeSP> entry_nodes{};
    mutable std::shared_mutex entry_nodes_mutex{};
    std::vector<NodeWP> all_nodes{};
    mutable std::shared_mutex all_nodes_mutex{};
};


