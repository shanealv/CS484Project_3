#include "NetworkNode.h"
#include "NetworkLink.h"
#include "RandomGen.h"
#include "Dispatcher.h"
#include "Job.h"
#include "Statistics.h"
#include <array>
#include <limits>
#include <cstdbool>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>
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
	RandomGen::Init(seed); // initialize the random number generator

	cout << filename << " " << seed << endl;

	ifstream source(filename);
	int numNodes, numEdges;

	if (ReadLine(source, numNodes, numEdges) == false) // read the first line
		return 1;

	cout << "Nodes: " << numNodes << " Edges: " << numEdges << endl;

	// these vectors always hold shared references to all the nodes and edges
	vector<shared_ptr<NetworkNode>> nodes(numNodes);
	vector<shared_ptr<NetworkLink>> edges(numEdges);

	// create nodes
	for (int i = 0; i < numNodes; i++)
		nodes[i] = make_shared<NetworkNode>(i);

	// create edges
	for (int i = 0; i < numEdges; i++)
	{
		int nodeA, nodeB;
		if (ReadLine(source, nodeA, nodeB) == false)
			return 1;
		edges[i] = shared_ptr<NetworkLink>(new NetworkLink(i, nodes[nodeA], nodes[nodeB]));
	}

	cout << "Created Graph" << endl;

	// floyd warshall algorithm for all shortests paths
	vector<vector<double>> dist(numNodes); // distance matrix
	vector<vector<int>> next(numNodes); // forwarding matrix
	for (int i = 0; i < numNodes; i++)
	{
		dist[i] = vector<double>(numNodes, numeric_limits<double>::infinity());
		next[i] = vector<int>(numNodes, -1);
	}

	// distance from node to itself is 0
	for (int i = 0; i < numNodes; i++)
		dist[i][i] = 0;

	// add all the edges
	for (int i = 0; i < numEdges; i++)
	{
		auto edge = edges[i];
		int u = edge->GetNodeAId();
		int v = edge->GetNodeBId();
		dist[u][v] = 1;
		dist[v][u] = 1;
		next[u][v] = v;
		next[v][u] = u;

		// also add edge to respective nodes
		nodes[u]->AddLink(i, v);
		nodes[v]->AddLink(i, u);
	}

	// dynamic algorithm for calculating min distances and next nodes
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

	// create the routing tables for each node
	for (int i = 0; i < numNodes; i++)
		nodes[i]->BuildTable(next);

	cout << "Created Routing Tables" << endl;

	//Designate 20 source/destination pairs
	int srcid, destid;
	for (int i = 0; i < 20; i++)
	{
		do //assure source doesn't send to self and pair doesn't exist
		{
			srcid = rand() % numNodes;
			destid = rand() % numNodes;
		} while (srcid == destid || Statistics::PairExists(srcid, destid));

		Statistics::AddPair(srcid, destid);
		//time 0 jobs: queue packet creation for each source
		nodes[srcid]->CreateAndSendPacket(destid);
	}
	//time 0 completed: each source node has sent a packet
	Dispatcher::IncrementTime();

	cout << endl << endl << "Created First Packets" << endl << endl << endl;

	//Simulate 1000 iterations of the network
	vector<shared_ptr<Job>> dueJobs;

	for (int time = 1; time < 200; time++)
	{
		//Do due jobs
		dueJobs = Dispatcher::GetDueJobs();
		for (int i = 0; i < dueJobs.size(); i++)
		{
			cout << "[main][" << setw(4) << time << "] got job of type ";
			//do job
			switch (dueJobs[i]->GetType())
			{
			case JobType::PacketCreation:
				cout << "Packet Creation" << endl;
				nodes[dueJobs[i]->GetNodeId()]->CreateAndSendPacket(dueJobs[i]->GetDestId());
				break;
			case JobType::PacketProcessing:
				cout << "Packet Processing" << endl;
				nodes[dueJobs[i]->GetNodeId()]->RoutePacket(dueJobs[i]->GetPacket());
				break;
			case JobType::PacketUpload:
				cout << "Packet Upload" << endl;
				edges[dueJobs[i]->GetLinkId()]->AddToInputQueue(dueJobs[i]->GetNodeId(), dueJobs[i]->GetPacket());
				break;
			case JobType::PacketDownload:
				cout << "Packet Download" << endl;
				edges[dueJobs[i]->GetLinkId()]->AddToOutputQueue(dueJobs[i]->GetNodeId(), dueJobs[i]->GetPacket());
				break;
			case JobType::None:
				cout << "None" << endl;
				break;
			default:
				cout << "unrecognized job" << endl;
				//exit(1);
			}
		}

		//Prepare for next iteration
		//propagate every edge
		cout << "[main][" << setw(4) << time << "] Propagating " << endl << endl;
		for (int i = 0; i < numEdges; i++)
			edges[i]->Propagate();
		//Other cleanup
		Dispatcher::IncrementTime();
		cout << endl << endl;
	}

	//Print statistics
	Statistics::PrintAllStats(nodes);
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
