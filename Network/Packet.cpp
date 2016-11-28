#include "Packet.h"
#include "Dispatcher.h"

int Packet::NextId = 0;
int Packet::NumArrived = 0;
int Packet::TotalDelay = 0;

int Packet::GetNextId()
{
	return Packet::NextId++;
}

Packet::Packet(int size, int destination)
{
	_id = Packet::GetNextId();
	_size = size;
	_destination = destination;
	_creationTime = 0;//Dispatcher::GetCurrentTime();
}

int Packet::GetSize()
{
	return _size;
}

int Packet::GetDestination()
{
	return _destination;
}

int Packet::GetCreationTime()
{
	return _creationTime;
}

void Packet::OnArrive()
{
	Packet::NumArrived++;
	Packet::TotalDelay += 1;//Dispatcher::GetCurrentTime() - _creationTime;
}

int Packet::GetTotalPackets()
{
	return Packet::NextId;
}

int Packet::GetTotalArrived()
{
	return Packet::NumArrived;
}

int Packet::GetTotalDelay()
{
	return Packet::TotalDelay;
}