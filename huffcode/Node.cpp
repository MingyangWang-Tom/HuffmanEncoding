#include "Node.h"

int Node::getId()
{
	return id;
}

Node* Node::getLeftChild()
{
	return leftChild;
}

Node* Node::getRightChild()
{
	return rightChild;
}

void Node::setLeftChild(Node* n)
{
	leftChild = n;
}

void Node::setRightChild(Node* n)
{
	rightChild = n;
}

void Node::setLetter(char c)
{
	letter = c;
}

char Node::getLetter()
{
	return letter;
}

Node::Node(int i, char c)
{
	id = i;
	letter = c;
	leftChild = nullptr;
	rightChild = nullptr;
	typeLeaf = true;
}

bool Node::isLeaf()
{
	return typeLeaf;
}

Node::Node(int i)
{
	id = i;
	typeLeaf = false;
	leftChild = nullptr;
	rightChild = nullptr;
}