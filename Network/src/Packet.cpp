#include "Packet.h"
#include "Dispatcher.h"
#include "Statistics.h"

int Packet::NextId = 0;
int Packet::NumArrived = 0;
int Packet::TotalDelay = 0;

int Packet::GetNextId()
{
	return Packet::NextId++;
}

Packet::Packet(int size, int source, int destination)
{
	_id = Packet::GetNextId();
	_size = size;
	_source = source;
	_destination = destination;
	_creationTime = Dispatcher::GetCurrentTime();
}

int Packet::GetSize()
{
	return _size;
}

int Packet::GetSource()
{
	return _source;
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
	int delay = Dispatcher::GetCurrentTime() - _creationTime;
	Packet::NumArrived++;
	Packet::TotalDelay += delay;
	Statistics::RegisterArrival(_source, _destination, delay);
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