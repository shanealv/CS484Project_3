#include "NetworkNode.h"
#include "NetworkLink.h"
#include "RandomGen.h"
#include <array>
#include <limits>
#include <cstdbool>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Job.h"
#include "Dispatcher.h"

using namespace std;

void PrintHelp();
bool ReadLine(ifstream& source, int& valA, int& valB);
bool ReadLine(ifstream& source, int& valA, long& valB);

struct SourceDestPair
{
	int _src;
	int _dest;
};

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
	
	// floyd warshall
	vector<vector<double>> dist(numNodes);
	vector<vector<int>> next(numNodes);
	for (int i = 0; i < numNodes; i++)
	{
		dist[i] = vector<double>(numNodes, numeric_limits<double>::infinity());
		next[i] = vector<int>(numNodes, -1);
	}
	
	for (int i = 0; i < numNodes; i++)
		dist[i][i] = 0;
	
	for (int i = 0; i < numEdges; i++)
	{
		auto edge = edges[i];
		int u = edge->GetNodeAId();
		int v = edge->GetNodeBId();
		dist[u][v] = 1;
		dist[v][u] = 1;
		next[u][v] = v;
		next[v][u] = u;
	}
	for (int i = 0; i < numNodes; i++)
	{
		for (int j = 0; j < numNodes; j++)
			cout << setw(4) << next[i][j] << " ";
		cout << endl;
	}
	cout << endl;
	
	for (int k = 0; k < numNodes; k++)
		for (int i = 0; i < numNodes; i++)
			for (int j = 0; j < numNodes; j++)
			{
				double newDist = dist[i][k] + dist[k][j];
				if (dist[i][j] > newDist)
				{
					dist[i][j] = newDist;
					next[i][j] = next[i][k];
				}
			}
	
	for (int i = 0; i < numNodes; i++)
	{
		for (int j = 0; j < numNodes; j++)
			cout << setw(4) << dist[i][j] << " ";
		cout << endl;
	}
	
	cout << endl;
	
	for (int i = 0; i < numNodes; i++)
	{
		for (int j = 0; j < numNodes; j++)
			cout << setw(4) << next[i][j] << " ";
		cout << endl;
	}
	
	//Designate 20 source/destination pairs
	vector<SourceDestPair*> pairs(20);
	int srcid, destid;
	SourceDestPair* sdtemp;
	for(int i = 0; i < 20; i++)
	{
		sdtemp = (SourceDestPair*)malloc(sizeof(SourceDestPair));
		do
		{
			srcid = rand() % numNodes;
			destid = rand() % numNodes;
		}while(srcid == destid);
		sdtemp->_src = srcid;
		sdtemp->_dest = destid;
		pairs[i] = sdtemp;
	}
	
	//Simulate 1000 iterations of the network
	Dispatcher dispatcher;
	vector<shared_ptr<Job>> dueJobs;
	for(int time = 0; time < 10000; time++)
	{
		//Queue future jobs
		
		//Do due jobs
		dueJobs = dispatcher.GetDueJobs();
		for(int i = 0; i < dueJobs.size(); i++)
		{
			//do job
		}
		
		//Prepare for next iteration
		dispatcher.IncrementTime();
	}
	
	free(sdtemp);
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