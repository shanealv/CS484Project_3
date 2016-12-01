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
#include "Statistics.h"

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
	
	//for (int i = 0; i < numEdges; i++)
	//	cout << edges[i] << endl;
	
	// floyd warshall
	vector<vector<double>> dist(numNodes);
	vector<vector<int>> next(numNodes);
	for (int i = 0; i < numNodes; i++)
	{
		dist[i] = vector<double>(numNodes, numeric_limits<double>::infinity());
		next[i] = vector<int>(numNodes, -1);
	}
	
	for (int i = 0; i < numNodes; i++) // distance to a node to itself is 0
		dist[i][i] = 0;
	
	for (int i = 0; i < numEdges; i++) // add each inter-node distance (1 for each)
	{
		auto edge = edges[i];
		int u = edge->GetNodeAId();
		int v = edge->GetNodeBId();
		dist[u][v] = 1;
		dist[v][u] = 1;
		next[u][v] = v;
		next[v][u] = u;
		
		// also add edge to respective nodes
		nodes[u]->AddLink(i, u, v);
		nodes[v]->AddLink(i, u, v);
	}
	
	//for (int i = 0; i < numNodes; i++) // print matrix of distances before floyd warshall
	//{
	//	for (int j = 0; j < numNodes; j++)
	//		cout << setw(4) << next[i][j] << " ";
	//	cout << endl;
	//}
	//cout << endl;
	
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
	
	for (int i = 0; i < numNodes; i++) // print matrix of distances after floyd warshall
	{
		for (int j = 0; j < numNodes; j++)
			cout << setw(4) << dist[i][j] << " ";
		cout << endl;
	}
	
	cout << endl;
	
	for (int i = 0; i < numNodes; i++) // print the next node for each src/dest pair
	{
		for (int j = 0; j < numNodes; j++)
			cout << setw(4) << next[i][j] << " ";
		cout << endl;
	}
	
	//Designate 20 source/destination pairs
	int srcid, destid;
	for(int i = 0; i < 20; i++)
	{
		do //assure source doesn't send to self and pair doesn't exist
		{
			srcid = rand() % numNodes;
			destid = rand() % numNodes;
		}while(srcid == destid && !Statistics::PairExists(srcid,destid));
		
		Statistics::AddPair(srcid, destid);
		//time 0 jobs: queue packet creation for each source
		nodes[srcid]->CreateAndSendPacket(destid);
	}
	//time 0 completed: each source node has sent a packet
	Dispatcher::IncrementTime();
	
	//Simulate 1000 iterations of the network
	vector<shared_ptr<Job>> dueJobs;
	shared_ptr<Packet> tempPack;
	
	for(int time = 1; time < 1000; time++)
	{
		//Do due jobs
		dueJobs = Dispatcher::GetDueJobs();
		for(int i = 0; i < dueJobs.size(); i++)
		{
			//do job
			switch(dueJobs[i]->GetType())
			{
				case JobType::PacketCreation:
					nodes[dueJobs[i]->GetNodeId()]->CreateAndSendPacket(dueJobs[i]->GetDestId());
					break;
				case JobType::PacketProcessing:
					tempPack = dueJobs[i]->GetPacket();
					nodes[dueJobs[i]->GetNodeId()]->RoutePacket(tempPack);
					break;
				case JobType::PacketUpload:
					tempPack = dueJobs[i]->GetPacket();
					edges[dueJobs[i]->GetLinkId()]->AddToInputQueue(dueJobs[i]->GetNodeId(),
						tempPack);
					break;
				case JobType::PacketDownload:
					tempPack = dueJobs[i]->GetPacket();
					edges[dueJobs[i]->GetLinkId()]->AddToInputQueue(dueJobs[i]->GetPacket()->GetDestination(),
						tempPack);
					break;
				case JobType::None:
					break;
				default:
					cout <<  "unrecognized job" << endl;
					exit(1);
			}
		}
		
		//Prepare for next iteration
		//propagate every ledge
		for(int i = 0; i < numEdges; i++)
			edges[i]->Propagate();
		//Other cleanup
		Dispatcher::IncrementTime();
		tempPack = NULL;
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