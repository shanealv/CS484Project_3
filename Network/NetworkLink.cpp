#include "NetworkLink.h"
#include "Packet.h"
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
	_bandwidth = 7;
	// TODO: generate bandwidth
}

double NetworkLink::GetBandwidth()
{
	return _bandwidth;
}

queue<weak_ptr<Packet>>& NetworkLink::GetInputQueue(int endNodeId)
{
	if (_nodeA->GetId == endNodeId)
		return _inputQueueA;
	else if (_nodeB->GetId == endNodeId)
		return _inputQueueB;
	else
		throw invalid_argument("endNodeId not found on this link");
}

std::queue<std::weak_ptr<Packet>>& NetworkLink::GetOutputQueue(int endNodeId)
{
	if (_nodeA->GetId == endNodeId)
		return _outputQueueA;
	else if (_nodeB->GetId == endNodeId)
		return _outputQueueB;
	else
		throw invalid_argument("endNodeId not found on this link");
}

void NetworkLink::AddToInputQueue(int endNodeId, std::shared_ptr<Packet>& packet)
{
	auto& queue = GetInputQueue(endNodeId);
	int size = queue.size();
	if (size < NetworkLink::QueueLimit)
	{
		queue.push(packet); // TODO queue action with dispatcher
	}
}

void NetworkLink::AddToOutputQueue(int endNodeId, std::shared_ptr<Packet>& packet)
{
	auto& queue = GetOutputQueue(endNodeId);
	int size = queue.size();
	if (size < NetworkLink::QueueLimit)
	{
		queue.push(packet); // TODO queue action with dispatcher
	}
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
			int delay = 4; // TODO calculate delay
			packet->AddDelay(delay);
			_totalDelay += delay;
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
			int delay = 4; // TODO calculate delay
			packet->AddDelay(delay);
			_totalDelay += delay;
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