#pragma once
#include "Packet.h"
#include <memory>

class Dispatcher
{
private:
	static int CurrentTime;
public:
	static void QueuePacketCreation(int nodeId, int destinationId);
	static void QueuePacketProcessing(int nodeId, std::shared_ptr<Packet> packet, int delay);
	static void QueuePacketUpload(int nodeId, int linkId, std::shared_ptr<Packet> packet, int delay);
	static void QueuePacketDownload(int linkId, std::shared_ptr<Packet> packet, int delay);
	static int GetCurrentTime();
};