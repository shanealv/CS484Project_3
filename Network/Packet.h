#pragma once

class Packet
{
private:
	static int NextId;
	static int NumArrived;
	int _size = -1;
	int _destination = -1;
	double _delay = 0.0;

	int GetNextId();
public:
	Packet(int size, int destination);
	int GetSize();
	int GetDestination();
	double GetDelay();
	void AddDelay(double delay);
	void SetDelay(double delay);
	void OnArrive();
	static int GetTotalPackets();
};