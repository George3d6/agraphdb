#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include <climits>
#include <experimental/type_traits>
#include <type_traits>
#include <boost/algorithm/string/join.hpp>
#include <iostream>

/**
 * A Node is a structure which holds links to other node and some data
 * Currently nodes are not thread safe and must rely on being managed by a graph in a multi-threaded enviornment
 */
template<class InternalDataType>
class Node {
public:
    using TypedNode = Node<InternalDataType>;
    using NodeSP = std::shared_ptr<TypedNode>;
    static std::string serialization_separator() { static std::string _r{"|"}; return _r; }
    static std::string serialization_link_list_separator() { static std::string _r{","}; return _r; }

    std::weak_ptr<TypedNode> self;
    std::vector<NodeSP> links{};
    InternalDataType internal_data{};
    uint64_t id;

    static NodeSP create(InternalDataType new_internal_data) {
        const NodeSP node_sp{new Node{new_internal_data}};
        node_sp->self = node_sp;
        return node_sp;
    }

    /// Returns a {meatadata, internal_data} pair where "metadata" holds logic information and deserialization information about the node
    /// and "internal_data" hold a serialization of the internal data held by the node
    std::pair<std::string, std::string> serialize_node() const noexcept {
        const NodeSP temp_sp = self.lock();
        if(temp_sp == nullptr) { return  std::make_pair("",""); }
        std::vector<std::string> links_id{};
        for(auto& link : links) {
            links_id.push_back(std::to_string(link->id));
        }

        const std::string internal_data_serialization = internal_data.serialize();
        return  std::make_pair(std::string{std::to_string(temp_sp->id) + TypedNode::serialization_separator()
                + boost::algorithm::join(links_id, TypedNode::serialization_link_list_separator())
                + TypedNode::serialization_separator() + std::to_string(internal_data_serialization.size())}
                , internal_data_serialization);

    }

private:
    Node(InternalDataType new_internal_data): internal_data(new_internal_data) {}
    static uint64_t uniq_id(bool increment = true) { static uint64_t _uniq_id = 0; if (increment){ ++_uniq_id; } return _uniq_id; }
};
