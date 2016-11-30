#include "Dispatcher.h"

using namespace std;

int Dispatcher::CurrentTime = 0;
priority_queue<Job, vector<shared_ptr<Job>>, DelayComparison> Dispatcher::JobQueue;

void Dispatcher::QueuePacketCreation(int nodeId, int destinationId)
{
	Job newJob(nodeId, destinationId, -1, NULL, CurrentTime, JobType::PacketCreation);
	shared_ptr<Job> ptr = make_shared<Job>(newJob);
	Dispatcher::JobQueue.push(ptr);
}

void Dispatcher::QueuePacketProcessing(int nodeId, std::shared_ptr<Packet> packet, int delay)
{
	Job newJob(nodeId, -1, -1, packet, CurrentTime + delay, JobType::PacketProcessing);
	shared_ptr<Job> ptr = make_shared<Job>(newJob);
	Dispatcher::JobQueue.push(ptr);
}

void Dispatcher::QueuePacketUpload(int nodeId, int linkId, std::shared_ptr<Packet> packet, int delay)
{
	Job newJob(nodeId, -1, linkId, packet, CurrentTime + delay, JobType::PacketUpload);
	shared_ptr<Job> ptr = make_shared<Job>(newJob);
	Dispatcher::JobQueue.push(ptr);
}

void Dispatcher::QueuePacketDownload(int linkId, std::shared_ptr<Packet> packet, int delay)
{
	Job newJob(-1, -1, linkId, packet, CurrentTime + delay, JobType::PacketDownload);
	shared_ptr<Job> ptr = make_shared<Job>(newJob);
	Dispatcher::JobQueue.push(ptr);
}

int Dispatcher::GetCurrentTime()
{
	return Dispatcher::CurrentTime;
}
