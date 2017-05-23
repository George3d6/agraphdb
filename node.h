#pragma once

#include <list>
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
    using ExternalNodePointer = std::shared_ptr<TypedNode>;
    static std::string serialization_separator() { return "#|#"; }
    static std::string serialization_link_list_separator() { return ","; }

    std::weak_ptr<TypedNode> self;
    std::atomic<std::list<ExternalNodePointer>*> links;
    std::atomic<InternalDataType*> internal_data;
    uint64_t id;

    static ExternalNodePointer create(InternalDataType * new_internal_data) {
        ExternalNodePointer node_sp = ExternalNodePointer{new TypedNode{new_internal_data}};
        node_sp->self = node_sp;
        return node_sp;
    }

    void modify_data(InternalDataType * new_internal_data) {
        //Use a temporary shared_ptr to itself instead of this since otherwise the destructor might be called whilst inside this method
        ExternalNodePointer temp_sp = self.lock();
        temp_sp->internal_data.exchange(new_internal_data);
    }

    void modify_links(std::list<ExternalNodePointer> * new_links) {
        ExternalNodePointer temp_sp = self.lock();
        temp_sp->links.exchange(new_links);
    }

    std::string serialize_node_metadata() {
        ExternalNodePointer temp_sp = self.lock();
        std::list<std::string> links_id{};
        for(auto& link : *temp_sp->links) {
            links_id.push_back(std::to_string(link->id));
        }
        std::string internal_data_size = std::to_string((*temp_sp->internal_data).size()*CHAR_BIT);
        return  std::string{std::to_string(temp_sp->id) + TypedNode::serialization_separator()
                + boost::algorithm::join(links_id, TypedNode::serialization_link_list_separator())
                + TypedNode::serialization_separator() + internal_data_size};

    }

    
    // @TODO find the safest reasonably fast way to free up all used ressource here
    ~Node() {
        delete links;
        delete internal_data;
    }

private:
    Node(InternalDataType * new_internal_data) {
        this->internal_data.store(new_internal_data);
        this->links.store(new std::list<ExternalNodePointer>{});
    }
    static uint64_t uniq_id(bool increment = true) { static uint64_t _uniq_id = 0; if (increment){ ++_uniq_id; } return _uniq_id; }
};
