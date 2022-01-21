#include <iostream>

struct Node
{
	int Value;

	Node* Next;

	Node(int value, Node* next) : Value(value), Next(next) {}
};

void Write(Node* node)
{
	while (node != nullptr)
	{
		std::cout << node->Value << "\n";
		node = node->Next;
	}
}

int main()
{
	Node* node1 = new Node(2, nullptr);
	Node* node = new Node(1, node1);

	Write(node);
}