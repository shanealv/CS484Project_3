#pragma once
#include "Packet.h"
#include "Job.h"
#include <memory>
#include <queue>

class Dispatcher
{
private:
	static int CurrentTime;
	static std::priority_queue<std::shared_ptr<Job>, std::vector<std::shared_ptr<Job>>, DelayComparison> JobQueue;
public:
	static void QueuePacketCreation(int nodeId, int destinationId, int delay);
	static void QueuePacketProcessing(int nodeId, std::shared_ptr<Packet> packet, int delay);
	static void QueuePacketUpload(int nodeId, int linkId, std::shared_ptr<Packet> packet, int delay);
	static void QueuePacketDownload(int linkId, std::shared_ptr<Packet> packet, int delay);
	static int GetCurrentTime();
	static void IncrementTime();
	static std::vector<std::shared_ptr<Job>> GetDueJobs();
};