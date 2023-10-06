#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <bitset>
#include <chrono>
#include "Node.h"
#include <unordered_map>
#include <thread>
#include <string>

using namespace std;

int charmap[256] = { 0 };
vector<Node*> nodes;
//map<char, string> encodings;
//unordered_map<char, string> encodings;
string originalFile;
string encodings[256];


// Read input file
string readFile(ifstream& ifs) {
	ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	vector<char> bytes(fileSize);
	ifs.read(bytes.data(), fileSize);

	return string(bytes.data(), fileSize);
}

// Calculate Frequency
void frequency()
{
	for (char c : originalFile) {
		charmap[c]++;
	}
}

// Compare Node / Sort
bool compareNode(Node* n, Node* n2) {
	return (n->getId() < n2->getId());
}

// Generate Tree
void generateTree() {
	// Create a node for each character and its frequency
	for (int i = 0; i < 256; i++) {
		if (charmap[i] > 0) {
			int count = charmap[i];
			Node* n = new Node(count, i);
			nodes.push_back(n);
		}
	}
	// Build the Huffman tree by merging nodes
	while (nodes.size() > 1) {
		sort(nodes.begin(), nodes.end(), compareNode);
		Node* n1 = nodes[0];
		Node* n2 = nodes[1];
		// Create a new node by combining the two smallest frequency nodes
		Node* n3 = new Node(n1->getId() + n2->getId());
		n3->setLeftChild(n1);
		n3->setRightChild(n2);
		// Remove the merged nodes from the node vector
		nodes.erase(remove(nodes.begin(), nodes.end(), n1), nodes.end());
		nodes.erase(remove(nodes.begin(), nodes.end(), n2), nodes.end());
		// Add the new merged node to the node vector
		nodes.push_back(n3);
	}
}

// Traverse the Huffman tree and generate encodings for each character
void traverseTree(Node* n, string cur_encoding) {
	// If a leaf node is reached, save the character encoding
	if (n->getLeftChild() == nullptr && n->getRightChild() == nullptr) {
		encodings[(unsigned char)n->getLetter()] = cur_encoding;
	}
	// If a non-leaf node is reached, continue traversing recursively
	else {
		// Traverse left child with a "0" appended to the current encoding
		traverseTree(n->getLeftChild(), cur_encoding + "0");
		// Traverse right child with a "1" appended to the current encoding
		traverseTree(n->getRightChild(), cur_encoding + "1");
	}
}


int main(int argc, char** argv)
{
	auto start = chrono::high_resolution_clock::now();
	auto step = chrono::high_resolution_clock::now();
	cout << "Reading in File... ";
	//no command line argument
	if (argc < 2) {
		cout << "Command line parameter error" << endl;
		return -1;
	}

	string filename = argv[1];
	ifstream ifs;
	ifs.open(filename, ios::in | ios::binary | ios::ate);

	//file not found
	if (!ifs) {
		cout << "Error opening file: " << filename << endl;
		return -1;
	}

	if (filename.substr(filename.length() - 3) != "txt") {
		cout << "Wrong input file format. Make sure it is .txt file" << endl;
		return -1;
	}

	// Save file content to a string
	originalFile = readFile(ifs);
	auto step1 = chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = step1 - step;
	cout << duration.count() << " sec" << endl;
	
	// Close input file stream
	ifs.close();
	

	// Calculate character frequencies
	step = chrono::high_resolution_clock::now();
	cout << "Calculating Frequencies... ";
	frequency();
	step1 = chrono::high_resolution_clock::now();
	duration = step1 - step;
	cout << duration.count() << " sec" << endl;
	

	// Generate Huffman tree
	step = chrono::high_resolution_clock::now();
	cout << "Generating Tree... ";
	generateTree();
	step1 = chrono::high_resolution_clock::now();
	duration = step1 - step;
	cout << duration.count() << " sec" << endl;
	

	// Traverse Huffman tree to generate encodings for each character
	step = chrono::high_resolution_clock::now();
	cout << "Extracting Encodings... ";
	traverseTree(nodes[0], "");
	step1 = chrono::high_resolution_clock::now();
	duration = step1 - step;
	cout << duration.count() << " sec" << endl;
	

	// Encode file using generated encodings
	step = chrono::high_resolution_clock::now();
	cout << "Encoding File... ";
	//using 2 threads
	string output = "";
	int halfway = originalFile.length() / 2;
	// create two threads, one for firsthalf and one for secondhalf
	thread firstHalfThread([&]() {
		for (int i = 0; i < halfway; i ++) {
			char c = originalFile[i];
			output += encodings[(unsigned char)c];
		}
		});

	string output2 = "";
	thread secondHalfThread([&]() {
		for (int i = halfway; i < originalFile.length(); i ++) {
			char c = originalFile[i];
			output2 += encodings[(unsigned char)c];
		}
		});

	// wait for both threads to finish
	firstHalfThread.join();
	secondHalfThread.join();

	//combine two outputs together
	output = output + output2;

	step1 = chrono::high_resolution_clock::now();
	duration = step1 - step;
	cout << duration.count() << " sec" << endl;
	

	// Create output file stream
	step = chrono::high_resolution_clock::now();
	cout << "Outputting Header... ";
	string outputfilename;
	stringstream t(filename);
	getline(t, outputfilename, '.');
	outputfilename += ".zip301";
	ofstream ofs;
	ofs.open(outputfilename, ios::out | ios::binary);

	// Write character encodings to output file
	for (int i = 0; i < 256; i++) {
		if (encodings[i].length() > 0) {
			//char c = encodings[i];
			string encoding = encodings[i];
			string blankSpace;
			if (char(i) == '\n') {
				blankSpace = "newline";
			}
			else if (char(i) == '\r') {
				blankSpace = "return";
			}
			else if (char(i) == ' ') {
				blankSpace = "space";
			}
			else if (char(i) == '\t') {
				blankSpace = "tab";
			}
			else {
				blankSpace = (char)i;
			}
			ofs << encoding << " " << blankSpace << endl;
		}
	}
	ofs << "*****" << endl;
	step1 = chrono::high_resolution_clock::now();
	duration = step1 - step;
	cout << duration.count() << " sec" << endl;
	
	
	// Write the size of the compressed file to the output file
	step = chrono::high_resolution_clock::now();
	cout << "Outputting Content... ";
	int size = output.length();
	ofs << size << endl;

	// Pad output string with zeros to a multiple of 8 bits
	int countOffset = (size % 8 == 0) ? 0 : 8 - (size % 8);
	output.append(countOffset, '0');
	

	// Write compressed file to output file
	int outputLength = output.length();
	const int bufferSize = 4096;
	char buffer[bufferSize];
	const uint8_t bitMask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

	int bufferPos = 0;
	for (int i = 0; i < outputLength; i += 8) {
		uint8_t out = 0;
		for (int j = 0; j < 8; ++j) {
			out |= (output[i + j] - '0') * bitMask[j];
		}
		buffer[bufferPos++] = out;
		if (bufferPos == bufferSize) {
			ofs.write(buffer, bufferSize);
			bufferPos = 0;
		}
	}
	if (bufferPos > 0) {
		ofs.write(buffer, bufferPos);
	}

	ofs.close();

	step1 = chrono::high_resolution_clock::now();
	duration = step1 - step;
	cout << duration.count() << " sec" << endl;
	//step = chrono::high_resolution_clock::now();

	
	step1 = chrono::high_resolution_clock::now();
	duration = step1 - start;
	cout << "Total Runtime... " << duration.count() << " sec" << endl;
}