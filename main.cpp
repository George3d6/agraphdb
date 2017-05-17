#include <node.h>
#include <iostream>

bool nodeCreateAndModifyTest() {
	auto myNode = Node<int>::create(20, {});
	std::cout << myNode->load()->payload << "\n";
	myNode->load()->modify(50, {});
	std::cout << myNode->load()->payload << "\n";
	return true;
}

//Used for testing only at this point
int main(int argc, char *argv[])
{
	nodeCreateAndModifyTest();
	return 1;
}


