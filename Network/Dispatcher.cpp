#include "Dispatcher.h"

int Dispatcher::CurrentTime = 0;

static void Dispatcher::QueuePacketCreation(int nodeId, int destinationId)
{

}

static void Dispatcher::QueuePacketProcessing(int nodeId, std::shared_ptr<Packet> packet, int delay)
{

}

static void Dispatcher::QueuePacketUpload(int nodeId, int linkId, std::shared_ptr<Packet> packet, int delay)
{

}

static void Dispatcher::QueuePacketDownload(int linkId, std::shared_ptr<Packet> packet, int delay)
{

}

static int Dispatcher::GetCurrentTime()
{
	return Dispatcher::CurrentTime;
}