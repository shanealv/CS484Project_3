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
	std::array<int, 150> _routingTable;  // change to access global table
	std::vector<std::tuple<int, int, int>> _links; // does not own pointer, but can reasonable expect them to exist
public:
	NetworkNode(int id);

	std::vector<std::tuple<int, int, int>>  GetLinks();
	void AddLink(int a, int b);
	void BuildTable(std::vector<std::vector<int>> shortestPathTable);

	void RoutePacket(std::shared_ptr<Packet> & packet);
	void CreateAndSendPacket(int destination);

	bool GetIsSourceOrDestination();
	void SetIsSourceOrDestination(bool isSourceOrDestination);

	int GetDroppedPackets();
	void DropPacket();

	int GetId();
};
