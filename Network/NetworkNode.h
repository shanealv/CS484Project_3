#pragma once

#include <array>
#include <cstdbool>
#include <list>
#include <memory>
#include <vector>
#include "Packet.h"

class NetworkNode
{
private:
	int _id;
	bool _isSourceOrDestination;
	int _totalPackets = 0;
	int _droppedPackets = 0;
	double _totalDelay = 0.0;
	std::array<std::array<int, 150>, 150> routingTable;
	std::vector<std::shared_ptr<NetworkLink>> _links;
public:
	std::vector<std::shared_ptr<NetworkLink>> GetLinks();
	void RoutePacket(std::shared_ptr<Packet> packet);
	void CreateAndSendPacket(int destination);

	bool GetIsSourceOrDestination();
	void SetIsSourceOrDestination(bool isSourceOrDestination);

	int GetId();
};
