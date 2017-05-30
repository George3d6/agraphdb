#pragma once

#include<unordered_map>
#include<memory>
#include<mutex>
#include<utility>
#include<shared_mutex>
#include<node.h>


template <class NodeData>
class Graph {
public:
    using TypedNode = Node<NodeData>;
    using TypedContent = NodeContent<NodeData>;
    using NodeSP = std::shared_ptr<TypedNode>;
    using NodeWP = std::weak_ptr<TypedNode>;
    using ConstNodeSP = std::shared_ptr<const TypedNode>;
    using ConstNodeWP = std::weak_ptr<const TypedNode>;

    std::unique_ptr<TypedContent> get_node(uint64_t id) {
        std::shared_lock<std::shared_mutex> rlock{all_nodes_mutex};
        auto parent_node_wp_pair = all_nodes.find(id);
        if(parent_node_wp_pair == all_nodes.end()) {
            return nullptr;
        }
        return std::make_unique<TypedContent>(parent_node_wp_pair->second.lock());
    }

    void add_entry_node(NodeData data) noexcept {
        auto node_sp = TypedNode::create(data);
        std::unique_lock<std::shared_mutex> wlock{all_nodes_mutex};
        all_nodes.insert(std::make_pair(node_sp->id, node_sp));
        entry_nodes.push_back(std::move(node_sp));
    }

    template <class Container>
    void add_entry_nodes(Container data_container) noexcept {
        std::unique_lock<std::shared_mutex> wlock{all_nodes_mutex};
        for(auto data : data_container) {
            NodeSP node_sp = TypedNode::create(data);
            all_nodes.insert(std::make_pair(node_sp->id, node_sp));
            entry_nodes.push_back(std::move(node_sp));
        }
    }


    template<typename Function>
    std::unique_ptr<TypedContent> get_first_where(Function pick) const {
       std::shared_lock<std::shared_mutex> rlock{all_nodes_mutex};
       for(const auto& pair : all_nodes) {
            ConstNodeSP node_sp = pair.second.lock();
            if(node_sp != nullptr) {
                if(pick(node_sp->internal_data)) {
                    return std::make_unique<TypedContent>(node_sp) ;
                }
            }
       }
       return nullptr;
    }

    template<template <typename...> class Container, typename Function>
    Container<TypedContent> get_all_where(Function pick) const {
       Container<TypedContent> new_nodes{};
       std::shared_lock<std::shared_mutex> rlock{this->all_nodes_mutex};
       for(const auto& pair : all_nodes) {
            ConstNodeSP node_sp = pair.second.lock();
            if(node_sp != nullptr) {
                if(pick(node_sp->internal_data)) {
                    new_nodes.push_back(TypedContent{node_sp});
                }
            }
       }
       return new_nodes;
    }

    bool add_child_node(NodeData data, uint64_t parent_id) noexcept {
        auto node_sp = TypedNode::create(data);
        std::unique_lock<std::shared_mutex> wlock{all_nodes_mutex};
        auto parent_node_wp_pair = all_nodes.find(parent_id);
        if(parent_node_wp_pair == all_nodes.end()) {
            return false;
        }
        auto parent_node_sp = parent_node_wp_pair->second.lock();
        if(parent_node_sp == nullptr) {
            return false;
        }
        parent_node_sp->links.push_back(node_sp);
        all_nodes.insert(std::make_pair(node_sp->id, node_sp));
        return true;
    }

    template <template <typename...> class Container>
    bool add_child_nodes(Container<NodeData> data_container, uint64_t parent_id) noexcept {
        std::unique_lock<std::shared_mutex> wlock{all_nodes_mutex};
        auto parent_node_wp_pair = all_nodes.find(parent_id);
        if(parent_node_wp_pair == all_nodes.end()) {
            return false;
        }
        auto parent_node_sp = parent_node_wp_pair->second.lock();
        if(parent_node_sp == nullptr) {
            return false;
        }

        for(auto&& data : data_container) {
            auto node_sp = TypedNode::create(data);
            parent_node_sp->links.push_back(node_sp);
            all_nodes.insert(std::make_pair(node_sp->id, node_sp));
        }
        return true;
    }

    bool link_nodes(uint64_t first, uint64_t second) {
        std::unique_lock<std::shared_mutex> wlock{all_nodes_mutex};
        auto first_wp_pair = all_nodes.find(first);
        auto second_wp_pair = all_nodes.find(second);
        if(first_wp_pair == all_nodes.end() || second_wp_pair == all_nodes.end()) {
            return false;
        }
       auto first_sp =  first_wp_pair->second.lock();
       auto second_sp = second_wp_pair->second.lock();

       if(first_sp == nullptr || second_sp == nullptr) {
           return false;
       }
       first_sp->links.push_back(std::move(second_sp));
       return true;
    }

    //Returns a single child of a node
    std::unique_ptr<TypedContent> get_single_kid(uint64_t id) const {
       std::shared_lock<std::shared_mutex> rlock{all_nodes_mutex};
       NodeSP node_sp = all_nodes.at(id).second.lock();
       if(node_sp->links.size() == 0) {
            return nullptr;
       }
       return TypedContent{node_sp->links.front()};
    }

    //Returns a container with all kids of a node
    template<template <typename...> class Container>
    Container<TypedContent> get_kids(uint64_t id) const {
       Container<TypedContent> new_nodes{};
       std::shared_lock<std::shared_mutex> rlock{this->all_nodes_mutex};
       NodeSP node_sp = all_nodes.at(id).second.lock();
       for(const auto & node : node_sp->links) {
           new_nodes.push_back(TypedContent{node});
       }
    }

private:
    std::vector<NodeSP> entry_nodes{};
    std::unordered_map<uint64_t, NodeWP> all_nodes{};
    //Alos used for entry node atm since I don't want to get into the whole double locking problem now
    mutable std::shared_mutex all_nodes_mutex{};
};


