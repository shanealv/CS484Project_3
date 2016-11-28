#pragma once
#include "Packet.h"
#include "Job.h"
#include <memory>
#include <queue>

class Dispatcher
{
private:
	static int CurrentTime;
	static std::priority_queue<Job, std::vector<Job>, DelayComparison> JobQueue;
public:
	static void QueuePacketCreation(int nodeId, int destinationId);
	static void QueuePacketProcessing(int nodeId, std::shared_ptr<Packet> packet, int delay);
	static void QueuePacketUpload(int nodeId, int linkId, std::shared_ptr<Packet> packet, int delay);
	static void QueuePacketDownload(int linkId, std::shared_ptr<Packet> packet, int delay);
	static int GetCurrentTime();
};