#include "Statistics.h"
#include <climits>

using namespace std;

vector<SourceDestPair*> Statistics::pairs;

void Statistics::RegisterCreation(int src, int dest)
{
	for(int i = 0; i < pairs.size(); i++)
		if(pairs[i]->_src == src && pairs[i]->_dest == dest)
		{
			pairs[i]->_attempted++;
			return;
		}
}

void Statistics::RegisterArrival(int src, int dest, int delay)
{
	for(int i = 0; i < pairs.size(); i++)
		if(pairs[i]->_src == src && pairs[i]->_dest == dest)
		{
			pairs[i]->_arrived++;
			pairs[i]->_totalDelay += delay;
			return;
		}
}

void Statistics::PrintAveDelay()
{
	for(int i = 0; i < pairs.size(); i++)
		cout << "\t(" << pairs[i]->_src << "," << pairs[i]->_dest
			<< "): " << pairs[i]->_totalDelay / pairs[i]->_totalDelay
			<< endl;
}

void Statistics::PrintLossRate()
{
	for(int i = 0; i < pairs.size(); i++)
		cout << "\t(" << pairs[i]->_src << "," << pairs[i]->_dest
			<< "): " << (double)(pairs[i]->_arrived) / pairs[i]->_attempted
			<< endl;
}

void Statistics::PrintMinMaxDropped(vector<shared_ptr<NetworkNode>> nodes)
{
	int max = INT_MIN, min = INT_MAX, ave = 0;
	int dropped;
	for(int i = 0; i < nodes.size(); i++)
	{
		dropped = nodes[i]->GetDroppedPackets();
		if(dropped > max)
			max = dropped;
		if(dropped < min)
			min = dropped;
		ave += dropped;
	}
	cout << "\tMax: " << max << " | Min: " << min << " | Ave: " << ave/nodes.size() << endl;
}

void Statistics::PrintAllStats(vector<shared_ptr<NetworkNode>> nodes)
{
	cout << "- Total number of packets generated by sources: " << Packet::GetTotalPackets() << endl;
	
	cout << "- Total number of packets which reached their destinations successfully: " << Packet::GetTotalArrived() << endl;
	
	cout << "- The overall average total delay for a packet (measured from source to destination): " << Packet::GetTotalDelay() / Packet::GetTotalArrived() << endl;
	
	cout << "- The average delay of the packets within each flow (per source-destination pair): " << endl;
		PrintAveDelay();
	
	cout << "- The packet loss rate, as a percentage, for each source-destination pair: " << endl;
		PrintLossRate();
	
	cout << "• The maximum, minimum, and average number of packets dropped per router: " << endl;
		PrintMinMaxDropped(nodes);
}

void Statistics::AddPair(int src, int dest)
{
	auto sdtemp = new SourceDestPair();
	
	if(PairExists(src, dest))
		return;
	
// 	sdtemp = (SourceDestPair*)malloc(sizeof(SourceDestPair));
	sdtemp->_src = src;
	sdtemp->_dest = dest;
	sdtemp->_arrived = 0;
	sdtemp->_attempted = 0;
	sdtemp->_totalDelay = 0;

	pairs.push_back(sdtemp);
}

bool Statistics::PairExists(int src, int dest)
{
	for(int i = 0; i < pairs.size(); i++)
		if(pairs[i]->_src == src && pairs[i]->_dest == dest)
			return true;
	
	return false;
}