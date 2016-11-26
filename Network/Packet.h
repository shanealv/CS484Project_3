#pragma once



class Packet
{
public:
	int _size = -1;
	int _destination = -1;
	double _delay = 0.0;
public:
	Packet(int size, int destination);
	int GetSize();
	int GetDestination();
	double GetDelay();
	void AddDelay(double delay);
	void SetDelay(double delay);
};