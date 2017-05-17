#pragma once

#include <list>
#include <memory>
#include <atomic>

template<typename InternalDataType>
class Node {
public:
	//typedefs
	typedef std::atomic<Node<InternalDataType>*> NodeMetadata;

	//Containers
	std::weak_ptr<NodeMetadata> self{};
	std::list<std::shared_ptr<NodeMetadata>> links{};
	InternalDataType payload;
	//Create
	static std::shared_ptr<std::atomic<Node<InternalDataType>*>> create(const InternalDataType payload, const std::list<std::shared_ptr<NodeMetadata>> pointers) {

		std::shared_ptr<NodeMetadata> sharedPointerToMetadata = std::make_shared<NodeMetadata>(new Node<InternalDataType>{payload, pointers});

		NodeMetadata nodeUniquePointer{};
		nodeUniquePointer.store(*sharedPointerToMetadata.get());
		nodeUniquePointer.load()->self = sharedPointerToMetadata;
		return sharedPointerToMetadata;
	}
	//Modify <-- Thread safe
	void modify(const InternalDataType payload, const std::list<std::shared_ptr<NodeMetadata>> pointers) {
		self.lock()->exchange(new Node(payload, pointers));
	}
private:
	Node() {}
	Node(InternalDataType payload, std::list<std::shared_ptr<NodeMetadata>> pointers): payload(payload), links(links) {}
};
