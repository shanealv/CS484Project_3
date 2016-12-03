#include "NetworkNode.h"
#include "NetworkLink.h"
#include "Packet.h"
#include "RandomGen.h"
#include "Dispatcher.h"
#include "Statistics.h"
#include <cstdbool>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

NetworkNode::NetworkNode(int id)
{
	_id = id;
}

std::vector<std::tuple<int, int>> NetworkNode::GetLinks()
{
	return _links;
}

void NetworkNode::AddLink(int linkId, int dest)
{
	_links.push_back(tuple<int, int>(linkId, dest));
}

void NetworkNode::BuildTable(vector<vector<int>>& networkRoutingTable)
{
	// initialize table 
	int numEntries = networkRoutingTable.size();
	_routingTable = vector<int>(numEntries, -1);

	// networkRoutingTable should denote to next node to 
	//   forward a packet to to reach a destination
	for (int i = 0; i < numEntries; i++)
	{
		// find the link that connects to the next node
		int next = networkRoutingTable[_id][i];
		for (auto link : _links)
		{
			int linkId, dest;
			tie(linkId, dest) = link;
			if (next == dest)
				_routingTable[i] = linkId;
		}
	}
}

void NetworkNode::RoutePacket(shared_ptr<Packet> packet)
{
	// extract destination and check for arrival
	int destination = packet->GetDestination();
	if (destination == _id)
	{
		packet->OnArrive();
		return;
	}

	// determine the next link to forward the packet
	int linkIdx = _routingTable[destination];

#ifdef DEBUG
	cout << "[NetworkNode][" << _id << "] Routing to " << packet->GetDestination() << " via " << linkIdx << endl;
#endif // DEBUG

	// queue up the packet to load into the input queue
	double processingDelay = RandomGen::Exponential(1.0);
	Dispatcher::QueuePacketUpload(_id, linkIdx, packet, (int)ceil(processingDelay));
}

void NetworkNode::CreateAndSendPacket(int destination)
{
#ifdef DEBUG
	cout << "[NetworkNode][" << _id << "] Sending to " << destination << endl;
#endif // DEBUG
	// create and route the packet
	int size = RandomGen::Uniform(0.1, 1);
	auto packet = make_shared<Packet>(size, _id, destination);
	RoutePacket(packet);

	// queue up next packet to be created
	double creationDelay = RandomGen::Exponential(0.5);
	Dispatcher::QueuePacketCreation(_id, destination, (int)ceil(creationDelay));

	// register the creation of the packet
	Statistics::RegisterCreation(_id, destination);
}

bool NetworkNode::GetIsSourceOrDestination()
{
	return _isSourceOrDestination;
}
void NetworkNode::SetIsSourceOrDestination(bool isSourceOrDestination)
{
	_isSourceOrDestination = isSourceOrDestination;
}

int NetworkNode::GetDroppedPackets()
{
	return _droppedPackets;
}

void NetworkNode::DropPacket()
{
#ifdef DEBUG
	cout << "[NetworkNode][" << _id << "] Dropped Packet" << endl;
#endif // DEBUG
	_droppedPackets++;
}

int NetworkNode::GetId()
{
	return _id;
}
