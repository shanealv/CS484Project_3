#include "NetworkNode.h"
#include "NetworkLink.h"
#include "Packet.h"
#include "RandomGen.h"
#include "Dispatcher.h"
#include <cstdbool>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

NetworkNode::NetworkNode(int id)
{
	_id = id;
}

vector<weak_ptr<NetworkLink>> NetworkNode::GetLinks()
{
	return _links;
}

void NetworkNode::AddLink(shared_ptr<NetworkLink>& link)
{
	_links.push_back(link);
}

void NetworkNode::RoutePacket(shared_ptr<Packet> & packet)
{
	int dest = packet->GetDestination();
	double processingDelay = RandomGen::Exponential(1.0);
	if (dest == _id)
	{
		packet->OnArrive();
		return;
	}

	int linkIdx = _routingTable[dest];

	if (auto link = _links[linkIdx].lock())
		link->AddToInputQueue(_id, packet); // TODO: change to queue up action for dispatch
	else
		cerr << "Link has expired" << endl;
}

void NetworkNode::CreateAndSendPacket(int destination)
{
	int size = RandomGen::Uniform(0.1, 1);
	auto packet = make_shared<Packet>(new Packet(size, destination));

	int linkIdx = _routingTable[destination];

	if (auto link = _links[linkIdx].lock())
		link->AddToInputQueue(_id, packet); // TODO: change to queue up action for dispatch
	else
		cerr << "Link has expired" << endl;
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