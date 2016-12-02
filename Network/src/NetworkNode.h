#pragma once

#include "NetworkLink.h"
#include "Packet.h"
#include <array>
#include <cstdbool>
#include <list>
#include <memory>
#include <tuple>
#include <vector>

class NetworkLink;

class NetworkNode 
{
private:
	int _id;
	bool _isSourceOrDestination;
	int _droppedPackets = 0;
	int _totalDelay = 0;
	std::vector<int> _routingTable;  // maps destinations to the indexes of the _links vector 
	std::vector<std::tuple<int, int>> _links; // link information, contains linkId destination pairs
public:
	NetworkNode(int id);

	std::vector<std::tuple<int, int>>  GetLinks();
	void AddLink(int linkId, int dest);
	void BuildTable(std::vector<std::vector<int>>& networkRoutingTable);

	void RoutePacket(std::shared_ptr<Packet> & packet);
	void CreateAndSendPacket(int destination);

	bool GetIsSourceOrDestination();
	void SetIsSourceOrDestination(bool isSourceOrDestination);

	int GetDroppedPackets();
	void DropPacket();

	int GetId();
};
