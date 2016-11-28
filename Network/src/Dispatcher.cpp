#include "Dispatcher.h"

using namespace std;

int Dispatcher::CurrentTime = 0;
priority_queue<Job, vector<Job>, DelayComparison> Dispatcher::JobQueue;

void Dispatcher::QueuePacketCreation(int nodeId, int destinationId)
{
	Job newJob(nodeId, destinationId, -1, NULL, CurrentTime, JobType::PacketCreation);
	Dispatcher::JobQueue.push(newJob);
}

void Dispatcher::QueuePacketProcessing(int nodeId, std::shared_ptr<Packet> packet, int delay)
{
	Job newJob(nodeId, -1, -1, packet, CurrentTime + delay, JobType::PacketProcessing);
	Dispatcher::JobQueue.push(newJob);
}

void Dispatcher::QueuePacketUpload(int nodeId, int linkId, std::shared_ptr<Packet> packet, int delay)
{
	Job newJob(nodeId, -1, linkId, packet, CurrentTime + delay, JobType::PacketUpload);
	Dispatcher::JobQueue.push(newJob);
}

void Dispatcher::QueuePacketDownload(int linkId, std::shared_ptr<Packet> packet, int delay)
{
	Job newJob(-1, -1, linkId, packet, CurrentTime + delay, JobType::PacketDownload);
	Dispatcher::JobQueue.push(newJob);
}

int Dispatcher::GetCurrentTime()
{
	return Dispatcher::CurrentTime;
}
