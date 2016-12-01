#include "NetworkNode.h"
#include "NetworkLink.h"
#include "Packet.h"
#include "RandomGen.h"
#include "Dispatcher.h"
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

std::vector<std::tuple<int, int, int>> NetworkNode::GetLinks()
{
	return _links;
}

void NetworkNode::AddLink(int id, int a, int b)
{
	_links.push_back(tuple<int, int, int>(id, a, b));
}

void NetworkNode::BuildTable(vector<vector<int>> shortestPathTable)
{
	for (int i = 0; i < 150; i++)
	{
		int next = shortestPathTable[_id][i];
		for (auto link : _links)
		{
			int id, a, b;
			tie(id, a, b) = link;
			if (id == next)
				_routingTable[i] = id;
		}
	}
}

void NetworkNode::RoutePacket(shared_ptr<Packet> & packet)
{
	int destination = packet->GetDestination();
	double processingDelay = RandomGen::Exponential(1.0);
	if (destination == _id)
	{
		packet->OnArrive();
		return;
	}

	// determine which link to use
	int linkIdx = _routingTable[destination];
	
	// queue up the packet to load into the input queue
	Dispatcher::QueuePacketUpload(_id, linkIdx, packet, (int) ceil(processingDelay));
}

void NetworkNode::CreateAndSendPacket(int destination)
{
	int size = RandomGen::Uniform(0.1, 1);
	auto packet = shared_ptr<Packet>(new Packet(size, _id, destination));

	// determine which link to use
	int linkIdx = _routingTable[destination];
	
	// queue up the packet to load into the input queue
	double processingDelay = RandomGen::Exponential(1.0);
	Dispatcher::QueuePacketUpload(_id, linkIdx, packet, (int) ceil(processingDelay));
	
	// queue up next packet to be created
	double creationDelay = RandomGen::Exponential(0.5);
	Dispatcher::QueuePacketCreation(_id, destination, (int) ceil(creationDelay)); 
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
	_droppedPackets++;
}

int NetworkNode::GetId()
{
	return _id;
}