#pragma once

#include "NetworkLink.h"
#include "Packet.h"
#include <array>
#include <cstdbool>
#include <list>
#include <memory>
#include <vector>

class NetworkLink;

class NetworkNode 
{
private:
	int _id;
	bool _isSourceOrDestination;
	int _droppedPackets = 0;
	int _totalDelay = 0;
	std::array<int, 150> _routingTable;
	std::vector<std::weak_ptr<NetworkLink>> _links; // does not own pointer, but can reasonable expect them to exist
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
