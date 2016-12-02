#include "Dispatcher.h"
#include "NetworkLink.h"
#include "NetworkNode.h"
#include "Packet.h"
#include "RandomGen.h"
#include <cmath>
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
	auto nodeA = _nodeA.lock(); // comment
	auto nodeB = _nodeB.lock();
	if (nodeA->GetId() == destinationId)
		return _outputQueueA;
	else if (nodeB->GetId() == destinationId)
		return _outputQueueB;
	else
		throw invalid_argument("destinationId not found on this link");
}

void NetworkLink::AddToInputQueue(int sourceId, std::shared_ptr<Packet> packet)
{
	if (packet.get() == NULL) 
	{
		cout << "[NetworkLink][" << _id << "] INPUT QUEUING : MISSING PACKET" << endl; 
		return;
	}
	cout << "[NetworkLink][" << _id << "] INPUT Queuing Packet heading to " << packet->GetDestination() << endl;
	if (_nodeA.expired() || _nodeB.expired()) { cout << "WEJRLKWEJRLEWJRWE" << endl; return; }
	auto nodeA = _nodeA.lock();
	auto nodeB = _nodeB.lock();
	auto& queue = GetInputQueue(sourceId);
	if (queue.size() < NetworkLink::QueueLimit)
	{
		queue.push(packet);
		return;
	}

	// drop packet
	if (nodeA->GetId() == sourceId)
		nodeA->DropPacket();
	else if (nodeB->GetId() == sourceId)
		nodeB->DropPacket();
}

void NetworkLink::AddToOutputQueue(int destinationId, std::shared_ptr<Packet> packet)
{
	if (packet.get() == NULL) 
	{
		cout << "[NetworkLink][" << _id << "] OUTPUT QUEUING : MISSING PACKET" << endl; 
		return;
	}
	cout << "[NetworkLink][" << _id << "] OUTPUT Queuing Packet heading to " << packet->GetDestination() << endl;
	if (_nodeA.expired() || _nodeB.expired()) { cout << "WEJRLKWEJRLEWJRWE" << endl; return; }
	auto nodeA = _nodeA.lock();
	auto nodeB = _nodeB.lock();
	auto& queue = GetOutputQueue(destinationId);
	int size = queue.size();
	if (size < NetworkLink::QueueLimit)
	{
		queue.push(packet);
		return;
	}

	// drop packet
	if (nodeA->GetId() == destinationId)
		nodeA->DropPacket();
	else if (nodeB->GetId() == destinationId)
		nodeB->DropPacket();
}

void NetworkLink::Propagate()
{
	if (_nodeA.expired() || _nodeB.expired()) { cout << "WEJRLKWEJRLEWJRWE" << endl; return; }

	auto nodeA = _nodeA.lock();
	auto nodeB = _nodeB.lock();
	// Input queue A to output queue B
	if (!_inputQueueA.empty())
	{
		auto packet = _inputQueueA.front();
		_inputQueueA.pop();
		cout << "[NetworkLink][" << _id << "] InputQueueA (" << nodeA->GetId() << ") Packet Found heading to " << packet->GetDestination() << endl;
		double pDelay = RandomGen::Uniform(1.0, 10.0); // propagation delay
		double tDelay = packet->GetSize() / _bandwidth; // transmission delay
		Dispatcher::QueuePacketDownload(nodeB->GetId(), _id, packet, (int)ceil(pDelay + tDelay));
	}

	// Input queue B to output queue A
	if (!_inputQueueB.empty())
	{
		auto packet = _inputQueueB.front();
		_inputQueueB.pop();
		cout << "[NetworkLink][" << _id << "] InputQueueB (" << nodeB->GetId() << ") Packet Found heading to " << packet->GetDestination() << endl;
		double pDelay = RandomGen::Uniform(1.0, 10.0); // propagation delay
		double tDelay = packet->GetSize() / _bandwidth; // transmission delay
		Dispatcher::QueuePacketDownload(nodeA->GetId(), _id, packet, (int)ceil(pDelay + tDelay));
	}

	// Output queue A to node A routing
	if (!_outputQueueA.empty())
	{
		auto packet = _outputQueueA.front();
		if (packet.get() == NULL) 
		{
			cout << "MISSING PACKET!" << endl;
			return;
		}
		_outputQueueA.pop();
		cout << "[NetworkLink][" << _id << "] OutputQueueA (" << nodeA->GetId() << ") Packet Found heading to " << packet->GetDestination() << endl;
		nodeA->RoutePacket(packet);
	}

	// Output queue B to node B routing
	if (!_outputQueueB.empty())
	{
		auto packet = _outputQueueB.front();
		if (packet.get() == NULL) 
		{
			cout << "MISSING PACKET!" << endl;
			return;
		}
		_outputQueueB.pop();
		cout << "[NetworkLink][" << _id << "] OutputQueueB (" << nodeB->GetId() << ") Packet Found heading to " << packet->GetDestination() << endl;
		nodeB->RoutePacket(packet);
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
	return os;
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
	return os;
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
