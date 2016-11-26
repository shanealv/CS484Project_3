
#include "Packet.h"

int Packet::NextId = 0;
int Packet::NumArrived = 0;

int Packet::GetNextId()
{
	return NextId++;
}

Packet::Packet(int size, int destination)
{
	_size = size;
	_destination = destination;
}

int Packet::GetSize()
{
	return _size;
}

int Packet::GetDestination()
{
	return _destination;
}

double Packet::GetDelay()
{
	return _delay;
}

void Packet::AddDelay(double delay)
{
	_delay += delay;
}

void Packet::SetDelay(double delay)
{
	_delay = delay;
}

void Packet::OnArrive()
{
	NumArrived++;
}

int Packet::GetTotalPackets()
{
	return NextId;
}