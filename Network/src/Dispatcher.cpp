#include "Dispatcher.h"

using namespace std;

int Dispatcher::CurrentTime = 0;
priority_queue<Job, vector<Job>, DelayComparison> JobQueue();

void Dispatcher::QueuePacketCreation(int nodeId, int destinationId)
{
	Job newJob(nodeId, destinationId, -1, NULL, CurrentTime, JobType::PacketCreation);
	//Dispatcher::JobQueue.push(newJob);
}

void Dispatcher::QueuePacketProcessing(int nodeId, std::shared_ptr<Packet> packet, int delay)
{
	Job newJob(nodeId, -1, -1, packet, CurrentTime + delay, JobType::PacketProcessing);
}

void Dispatcher::QueuePacketUpload(int nodeId, int linkId, std::shared_ptr<Packet> packet, int delay)
{
	Job newJob(nodeId, -1, linkId, packet, CurrentTime + delay, JobType::PacketUpload);
}

void Dispatcher::QueuePacketDownload(int linkId, std::shared_ptr<Packet> packet, int delay)
{
	Job newJob(-1, -1, linkId, packet, CurrentTime + delay, JobType::PacketDownload);
}

int Dispatcher::GetCurrentTime()
{
	return Dispatcher::CurrentTime;
}