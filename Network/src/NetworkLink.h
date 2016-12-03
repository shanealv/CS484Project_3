#pragma once

#include "NetworkNode.h"
#include "Packet.h"
#include <memory>
#include <queue>
#include <iostream>

class NetworkNode;

class NetworkLink
{
private:
	int _id;
	int _numPackets = 0;
	int _droppedPackets = 0;
	int _totalDelay = 0;
	double _bandwidth;
	int _inputDelayA;
	int _inputDelayB;
	std::weak_ptr<NetworkNode> _nodeA; // does not own pointer, but can reasonable expect them to exist
	std::weak_ptr<NetworkNode> _nodeB; // does not own pointer, but can reasonable expect them to exist
	std::queue<std::shared_ptr<Packet>> _inputQueueA;
	std::queue<std::shared_ptr<Packet>> _inputQueueB;
	std::queue<std::shared_ptr<Packet>> _outputQueueA;
	std::queue<std::shared_ptr<Packet>> _outputQueueB;
public:
	NetworkLink(int id, std::shared_ptr<NetworkNode>& nodeA, std::shared_ptr<NetworkNode>& nodeB);
	static const int QueueLimit = 30;
	double GetBandwidth();
	
	int GetNodeAId();
	int GetNodeBId();
	
	std::queue<std::shared_ptr<Packet>>& GetInputQueue(int sourceId);
	std::queue<std::shared_ptr<Packet>>& GetOutputQueue(int destinationId);

	void AddToInputQueue(int sourceId, std::shared_ptr<Packet> packet);
	void AddToOutputQueue(int destinationId, std::shared_ptr<Packet> packet);

	void Propagate();

	int GetNumPackets();
	int GetDroppedPackets();
	double GetTotalDelay();

	friend std::ostream& operator<<(std::ostream& os, NetworkLink& link);

	int GetId();
};

std::ostream& operator<<(std::ostream& os, std::shared_ptr<NetworkLink>& link);
