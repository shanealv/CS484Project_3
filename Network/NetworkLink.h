#pragma once

#include <memory>
#include <queue>
#include "NetworkNode.h"
#include "Packet.h"

class NetworkLink
{
private:
	int _id;
	double _bandwidth;
	std::shared_ptr<NetworkNode> _nodeA;
	std::shared_ptr<NetworkNode> _nodeB;
	std::queue<std::shared_ptr<Packet>> _inputQueueA;
	std::queue<std::shared_ptr<Packet>> _inputQueueB;
	std::queue<std::shared_ptr<Packet>> _outputQueueA;
	std::queue<std::shared_ptr<Packet>> _outputQueueB;
public:
	static const int QueueLimit = 30;
	double GetBandwidth();
	std::queue<std::shared_ptr<Packet>> GetInputQueueA();
	std::queue<std::shared_ptr<Packet>> GetInputQueueB();
	std::queue<std::shared_ptr<Packet>> GetOutputQueueA();
	std::queue<std::shared_ptr<Packet>> GetOutputQueueB();

	void AddToInputQueueA (std::shared_ptr<Packet> packet);
	void AddToInputQueueB (std::shared_ptr<Packet> packet);
	void AddToOutputQueueA(std::shared_ptr<Packet> packet);
	void AddToOutputQueueB(std::shared_ptr<Packet> packet);

	int GetId();
};