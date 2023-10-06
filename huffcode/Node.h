#pragma once
class Node
{
private:
	int id;
	char letter;
	Node* leftChild;
	Node* rightChild;
	bool typeLeaf;
public:
	int getId();
	Node* getLeftChild();
	Node* getRightChild();
	void setLeftChild(Node*);
	void setRightChild(Node*);
	void setLetter(char);
	char getLetter();
	Node(int, char);
	bool isLeaf();
	Node(int);
};