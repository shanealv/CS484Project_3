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
	int _droppedPackets = 0;
	int _totalDelay = 0;
	std::array<int, 150> _routingTable;
	std::vector<std::weak_ptr<NetworkLink>> _links;
public:
	NetworkNode(int id);

	std::vector<std::weak_ptr<NetworkLink>> GetLinks();
	void AddLink(std::shared_ptr<NetworkLink> & link);

	void RoutePacket(std::shared_ptr<Packet> & packet);
	void CreateAndSendPacket(int destination);

	bool GetIsSourceOrDestination();
	void SetIsSourceOrDestination(bool isSourceOrDestination);

	int GetDroppedPackets();
	void DropPacket();

	int GetId();
};
