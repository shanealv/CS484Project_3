#include "NetworkLink.h"
#include "NetworkNode.h"
#include "Packet.h"
#include "RandomGen.h"
#include <exception>
#include <iostream>
#include <memory>
#include <queue>

using namespace std;

NetworkLink::NetworkLink(int id, shared_ptr<NetworkNode>& nodeA, shared_ptr<NetworkNode>& nodeB)
{
	_nodeA = nodeA;
	_nodeB = nodeB;
	_id = id;
	_bandwidth = RandomGen::Uniform();
	if (_bandwidth == 0.0)
		_bandwidth = 1.0;
}

double NetworkLink::GetBandwidth()
{
	return _bandwidth;
}

queue<shared_ptr<Packet>>& NetworkLink::GetInputQueue(int sourceId)
{
	auto nodeA = _nodeA.lock();
	auto nodeB = _nodeB.lock();
	if (nodeA->GetId() == sourceId)
		return _inputQueueA;
	else if (nodeB->GetId() == sourceId)
		return _inputQueueB;
	else
		throw invalid_argument("sourceId not found on this link");
}

std::queue<std::shared_ptr<Packet>>& NetworkLink::GetOutputQueue(int destinationId)
{
	auto nodeA = _nodeA.lock();
	auto nodeB = _nodeB.lock();
	if (nodeA->GetId() == destinationId)
		return _outputQueueA;
	else if (nodeB->GetId() == destinationId)
		return _outputQueueB;
	else
		throw invalid_argument("destinationId not found on this link");
}

void NetworkLink::AddToInputQueue(int sourceId, std::shared_ptr<Packet>& packet)
{
	auto nodeA = _nodeA.lock();
	auto nodeB = _nodeB.lock();
	auto& queue = GetInputQueue(sourceId);
	int size = queue.size();
	if (size < NetworkLink::QueueLimit)
	{
		queue.push(packet); // TODO queue action with dispatcher
		return;
	}

	// drop packet
	if (nodeA->GetId() == sourceId)
		nodeA->DropPacket();
	else if (nodeB->GetId() == sourceId)
		nodeB->DropPacket();
}

void NetworkLink::AddToOutputQueue(int destinationId, std::shared_ptr<Packet>& packet)
{
	auto nodeA = _nodeA.lock();
	auto nodeB = _nodeB.lock();
	auto& queue = GetOutputQueue(destinationId);
	int size = queue.size();
	if (size < NetworkLink::QueueLimit)
	{
		queue.push(packet); // TODO queue action with dispatcher
	}

	// drop packet
	if (nodeA->GetId() == destinationId)
		nodeA->DropPacket();
	else if (nodeB->GetId() == destinationId)
		nodeB->DropPacket();
}

void NetworkLink::Propagate()
{
	auto nodeA = _nodeA.lock();
	auto nodeB = _nodeB.lock();
	// Input A -> Output B
	if (!_inputQueueA.empty())
	{
		auto packet = _inputQueueA.front();
		_inputQueueA.pop();
		int propagationDelay = RandomGen::Uniform(1.0, 10.0);
		_totalDelay += propagationDelay;
		_numPackets++;
		if (_outputQueueB.size() < NetworkLink::QueueLimit)
			_outputQueueB.push(packet); // TODO queue action with dispatcher using delay
		else
		{
			_droppedPackets++;
			nodeB->DropPacket();
		}
	}

	// Input B -> Output A
	if (!_inputQueueB.empty())
	{
		auto packet = _inputQueueB.front();
		_inputQueueB.pop();
		int propagationDelay = 4; RandomGen::Uniform(1.0, 10.0);
		_totalDelay += propagationDelay;
		_numPackets++;
		if (_outputQueueA.size() < NetworkLink::QueueLimit)
			_outputQueueA.push(packet); // TODO queue action with dispatcher using delay
		else
		{
			_droppedPackets++;
			nodeA->DropPacket();
		}
	}
}

int NetworkLink::GetNumPackets()
{
	return _numPackets;
}

int NetworkLink::GetDroppedPackets()
{
	return _droppedPackets;
}

double NetworkLink::GetTotalDelay()
{
	return _totalDelay;
}

ostream& operator<<(ostream& os, shared_ptr<NetworkLink>& link)
{
	os << *link.get();
}

ostream& operator<<(ostream& os, NetworkLink& link)
{
	os << "Link: " << link._id;
	os << " A: " << link.GetNodeAId();
	os << " B: " << link.GetNodeBId();
	os << " Pkts: " << link._numPackets;
	os << " Drop: " << link._droppedPackets;
	os << " Delay: " << link._totalDelay;
	os << " Bandw: " << link._bandwidth;
}


int NetworkLink::GetId()
{
	return _id;
}

int NetworkLink::GetNodeAId()
{
	return _nodeA.lock()->GetId();
}

int NetworkLink::GetNodeBId()
{
	return _nodeB.lock()->GetId();
}