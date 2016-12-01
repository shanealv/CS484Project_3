#pragma once
#include <stdio.h>
#include <vector>
#include <iomanip>
#include <iostream>
#include <string>
#include "Packet.h"
#include "NetworkNode.h"

struct SourceDestPair
{
	int _src;
	int _dest;
	int _arrived;
	int _attempted;
	int _totalDelay;
};

class Statistics
{
private:
	static std::vector<SourceDestPair*> pairs;
public:
	//Utility Methods
	static void RegisterCreation(int src, int dest);
	static void RegisterArrival(int src, int dest, int delay);
	static void PrintAveDelay();
	static void PrintLossRate();
	static void PrintMinMaxDropped(std::vector<std::shared_ptr<NetworkNode>> nodes);
	static void PrintAllStats(std::vector<std::shared_ptr<NetworkNode>> nodes);
	static void AddPair(int src, int dest);
	static bool PairExists(int src, int dest);
};
