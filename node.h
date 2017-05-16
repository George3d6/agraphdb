#pragma once

#include <list>
#include <memory>
#include <atomic>

template<typename InternalDataType>
class Node {
public:
	//Containers
	std::weak_ptr<std::atomic<Node<InternalDataType>*>> self{};
	std::list<std::shared_ptr<std::atomic<Node<InternalDataType>*>>> links{};
	InternalDataType payload;
	//Create
	static std::shared_ptr<std::atomic<Node<InternalDataType>*>> create() {
		std::atomic<Node<InternalDataType>*> nodeUniquePointer;
		nodeUniquePointer.store(new Node<InternalDataType>{});
		std::shared_ptr<std::atomic<Node<InternalDataType>*>> sharedPointerToMetadata = std::make_shared<std::atomic<Node<InternalDataType>*>>(nodeUniquePointer);
		(*sharedPointerToMetadata).load()->self = sharedPointerToMetadata;
		return sharedPointerToMetadata;
	}
	//Modify <-- Thread safe
	void modify(const Node & newNode) {
		(*(self)).exchange(new Node(newNode));
	}
private:
	Node() {}
};
