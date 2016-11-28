#pragma once

class Packet
{
private:
	static int NextId;
	static int NumArrived;
	static int TotalDelay;
	int _id;
	int _size;
	int _destination;
	int _creationTime;

	int GetNextId();
public:
	Packet(int size, int destination);
	int GetSize();
	int GetDestination();
	int GetCreationTime();
	void OnArrive();
	static int GetTotalPackets();
	static int GetTotalArrived();
	static int GetTotalDelay();
};