#pragma once

#include <memory>
#include <queue>
#include "NetworkNode.h"
#include "Packet.h"

class NetworkLink
{
private:
	int _id;
	int _numPackets;
	int _droppedPackets;
	int _totalDelay;
	double _bandwidth;
	std::shared_ptr<NetworkNode> _nodeA;
	std::shared_ptr<NetworkNode> _nodeB;
	std::queue<std::weak_ptr<Packet>> _inputQueueA;
	std::queue<std::weak_ptr<Packet>> _inputQueueB;
	std::queue<std::weak_ptr<Packet>> _outputQueueA;
	std::queue<std::weak_ptr<Packet>> _outputQueueB;
public:
	NetworkLink(int id, std::shared_ptr<NetworkNode>& nodeA, std::shared_ptr<NetworkNode>& nodeB);
	static const int QueueLimit = 30;
	double GetBandwidth();
	std::queue<std::weak_ptr<Packet>>& GetInputQueue(int sourceId);
	std::queue<std::weak_ptr<Packet>>& GetOutputQueue(int destinationId);

	void AddToInputQueue(int sourceId, std::shared_ptr<Packet>& packet);
	void AddToOutputQueue(int destinationId, std::shared_ptr<Packet>& packet);

	void Propagate();

	int GetNumPackets();
	int GetDroppedPackets();
	double GetTotalDelay();

	int GetId();
};