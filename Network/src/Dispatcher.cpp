#include "Dispatcher.h"

int Dispatcher::CurrentTime = 0;

void Dispatcher::QueuePacketCreation(int nodeId, int destinationId)
{

}

void Dispatcher::QueuePacketProcessing(int nodeId, std::shared_ptr<Packet> packet, int delay)
{

}

void Dispatcher::QueuePacketUpload(int nodeId, int linkId, std::shared_ptr<Packet> packet, int delay)
{

}

void Dispatcher::QueuePacketDownload(int linkId, std::shared_ptr<Packet> packet, int delay)
{

}

int Dispatcher::GetCurrentTime()
{
	return Dispatcher::CurrentTime;
}