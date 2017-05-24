#pragma once

#include<vector>
#include<memory>

#include<node.h>

template <class NodeData>
class Graph {
public:
    using TypedNode = Node<NodeData>;
    using NodePointer = std::shared_ptr<Node<NodeData>>;
    std::vector<NodePointer> entry_nodes{};
};
