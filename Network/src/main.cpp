#include "NetworkNode.h"
#include "NetworkLink.h"
#include "RandomGen.h"
#include <array>
#include <cstdbool>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

void PrintHelp();
bool ReadLine(ifstream& source, int& valA, int& valB);
bool ReadLine(ifstream& source, int& valA, long& valB);

int main(int argc, char* argv[])
{
	char * filename;
	long seed;

	if (argc != 3)
	{
		PrintHelp();
		return 0;
	}

	filename = argv[1];
	seed = stol(argv[2]);

	if (seed == -1)
	{
		cout << "Invalid Seed" << endl;
		return 0;
	}
	RandomGen::Init(seed);

	cout << filename << " " << seed << endl;

	ifstream source(filename);
	int numNodes;
	int numEdges;

	if (ReadLine(source, numNodes, numEdges) == false)
		return 1;

	vector<shared_ptr<NetworkNode>> nodes(numNodes); // shared_ptr owners
	vector<shared_ptr<NetworkLink>> edges(numEdges); // shared_ptr owners

	for (int i = 0; i < numNodes; i++)
		nodes[i] = make_shared<NetworkNode>(i);

	for (int i = 0; i < numEdges; i++)
	{
		int nodeA, nodeB;
		if (ReadLine(source, nodeA, nodeB) == false)
			return 1;
		edges[i] = shared_ptr<NetworkLink>(new NetworkLink(i, nodes[nodeA], nodes[nodeB]));
	}

	for (int i = 0; i < numEdges; i++)
		cout << edges[i] << endl;

	array<vector<int
}

void PrintHelp()
{
	cout << "Usage:";
	cout << "\t./simulator file seed" << endl;
}

bool ReadLine(ifstream& source, int& valA, int& valB)
{
	string line;
	string cell;
	try
	{
		getline(source, line);
		stringstream linestream(line);
		getline(linestream, cell, ' ');
		valA = stoi(cell);
		getline(linestream, cell, ' ');
		valB = stoi(cell);
		if (valA < 0 || valB < 0)
		{
			cout << "Bad Parse" << endl;
			return false;
		}
	}
	catch (...)
	{
		cout << "Invalid Line" << endl;
		return false;
	}
	return true;
}

bool ReadLine(ifstream& source, int& valA, long& valB)
{
	string line;
	string cell;
	try
	{
		getline(source, line);
		stringstream linestream(line);
		getline(linestream, cell, ' ');
		valA = stoi(cell);
		getline(linestream, cell, ' ');
		valB = stol(cell);
		if (valA < 0 || valB < 0)
		{
			cout << "Bad Parse" << endl;
			return false;
		}
	}
	catch (...)
	{
		cout << "Invalid Line" << endl;
		return false;
	}
	return true;
}