#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include <climits>
#include <experimental/type_traits>
#include <type_traits>
#include <boost/algorithm/string/join.hpp>
#include <iostream>

template<class InternalDataType>
class Node {
public:
    using TypedNode = Node<InternalDataType>;
    using NodeSP = std::shared_ptr<TypedNode>;
    static std::string serialization_separator() { static std::string _r{"|"}; return _r; }
    static std::string serialization_link_list_separator() { static std::string _r{","}; return _r; }

    std::weak_ptr<TypedNode> self;
    std::atomic<const std::vector<NodeSP>*> links;
    std::atomic<const InternalDataType*> internal_data;
    uint64_t id;

    static NodeSP create(InternalDataType * new_internal_data) {
        const NodeSP node_sp = NodeSP{new TypedNode{new_internal_data}};
        node_sp->self = node_sp;
        return node_sp;
    }

    void modify_data(InternalDataType * new_internal_data) noexcept {
        //Use a temporary shared_ptr to itself instead of this since otherwise the destructor might be called whilst inside this method
        const NodeSP temp_sp = self.lock();
        if(temp_sp == nullptr) { return; }
        temp_sp->internal_data.exchange(new_internal_data);
    }

    void modify_links(std::vector<NodeSP> * new_links) noexcept {
        const NodeSP temp_sp = self.lock();
        if(temp_sp == nullptr) { return; }
        temp_sp->links.exchange(new_links);
    }

    void add_links(std::vector<NodeSP> * new_links) noexcept {
        const NodeSP temp_sp = self.lock();
        if(temp_sp == nullptr) { return; }
        new_links->insert(new_links->end(), temp_sp->links.load()->begin(), temp_sp->links.load()->end());
        temp_sp->links.exchange(new_links);
    }

    std::pair<std::string, std::string> serialize_node() const noexcept {
        const NodeSP temp_sp = self.lock();
        if(temp_sp == nullptr) { return  std::make_pair("",""); }
        std::vector<std::string> links_id{};
        for(auto& link : *temp_sp->links) {
            links_id.push_back(std::to_string(link->id));
        }

        const std::string internal_data_serialization = temp_sp->internal_data.load()->serialize();
        return  std::make_pair(std::string{std::to_string(temp_sp->id) + TypedNode::serialization_separator()
                + boost::algorithm::join(links_id, TypedNode::serialization_link_list_separator())
                + TypedNode::serialization_separator() + std::to_string(internal_data_serialization.size())}
                , internal_data_serialization);

    }

    
    // Does stating this disable move operations for Node... tbs
    ~Node() noexcept {
        delete links;
        delete internal_data;
    }

private:
    Node(InternalDataType * new_internal_data) noexcept {
        this->internal_data.store(new_internal_data);
        this->links.store(new std::vector<NodeSP>{});
    }
    static uint64_t uniq_id(bool increment = true) { static uint64_t _uniq_id = 0; if (increment){ ++_uniq_id; } return _uniq_id; }
};
