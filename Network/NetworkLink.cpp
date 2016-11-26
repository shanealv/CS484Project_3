#include "NetworkLink.h"
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
	_droppedPackets = 0;
	_bandwidth = RandomGen::Uniform();
	if (_bandwidth == 0.0)
		_bandwidth = 1.0;
}

double NetworkLink::GetBandwidth()
{
	return _bandwidth;
}

queue<weak_ptr<Packet>>& NetworkLink::GetInputQueue(int sourceId)
{
	if (_nodeA->GetId == sourceId)
		return _inputQueueA;
	else if (_nodeB->GetId == sourceId)
		return _inputQueueB;
	else
		throw invalid_argument("sourceId not found on this link");
}

std::queue<std::weak_ptr<Packet>>& NetworkLink::GetOutputQueue(int destinationId)
{
	if (_nodeA->GetId == destinationId)
		return _outputQueueA;
	else if (_nodeB->GetId == destinationId)
		return _outputQueueB;
	else
		throw invalid_argument("destinationId not found on this link");
}

void NetworkLink::AddToInputQueue(int sourceId, std::shared_ptr<Packet>& packet)
{
	auto& queue = GetInputQueue(sourceId);
	int size = queue.size();
	if (size < NetworkLink::QueueLimit)
	{
		queue.push(packet); // TODO queue action with dispatcher
		return;
	}

	// drop packet
	if (_nodeA->GetId == sourceId)
		_nodeA->DropPacket();
	else if (_nodeB->GetId == sourceId)
		_nodeB->DropPacket();
}

void NetworkLink::AddToOutputQueue(int destinationId, std::shared_ptr<Packet>& packet)
{
	auto& queue = GetOutputQueue(destinationId);
	int size = queue.size();
	if (size < NetworkLink::QueueLimit)
	{
		queue.push(packet); // TODO queue action with dispatcher
	}

	// drop packet
	if (_nodeA->GetId == destinationId)
		_nodeA->DropPacket();
	else if (_nodeB->GetId == destinationId)
		_nodeB->DropPacket();
}

void NetworkLink::Propagate()
{
	// Input A -> Output B
	if (!_inputQueueA.empty())
	{
		auto weakPacket = _inputQueueA.front();
		_inputQueueA.pop();
		if (auto packet = weakPacket.lock())
		{
			int propagationDelay = RandomGen::Uniform(1.0, 10.0);
			_totalDelay += propagationDelay;
			_numPackets++;
			if (_outputQueueB.size() < NetworkLink::QueueLimit)
				_outputQueueB.push(packet); // TODO queue action with dispatcher using delay
			else
			{
				_droppedPackets++;
				_nodeB->DropPacket();
			}
		}
		else
			cerr << "Packet has Expired" << endl;
	}

	// Input B -> Output A
	if (!_inputQueueB.empty())
	{
		auto weakPacket = _inputQueueB.front();
		_inputQueueB.pop();
		if (auto packet = weakPacket.lock())
		{
			int propagationDelay = 4; RandomGen::Uniform(1.0, 10.0);
			_totalDelay += propagationDelay;
			_numPackets++;
			if (_outputQueueA.size() < NetworkLink::QueueLimit)
				_outputQueueA.push(packet); // TODO queue action with dispatcher using delay
			else
			{
				_droppedPackets++;
				_nodeA->DropPacket();
			}
		}
		else
			cerr << "Packet has Expired" << endl;
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

int NetworkLink::GetId()
{
	return _id;
}