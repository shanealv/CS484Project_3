#include "Dispatcher.h"
#include "Job.h"
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

using namespace std;

int Dispatcher::CurrentTime = 0;
priority_queue<shared_ptr<Job>, vector<shared_ptr<Job>>, DelayComparison> Dispatcher::JobQueue;

void Dispatcher::QueuePacketCreation(int nodeId, int destinationId, int delay)
{
	int time = CurrentTime + delay;
	cout << "[Dispatcher] Creating Packet " << nodeId << " to " << destinationId << " to execute at time " << time << endl;
	auto emptyPacket = make_shared<Packet>(0, -1, -1);
	auto job = make_shared<Job>(nodeId, destinationId, -1, emptyPacket, time, JobType::PacketCreation);
	Dispatcher::JobQueue.push(job);
}

void Dispatcher::QueuePacketProcessing(int nodeId, shared_ptr<Packet> packet, int delay)
{
	int time = CurrentTime + delay;
	cout << "[Dispatcher] Processing Packet at node " << nodeId << " to execute at time " << time << endl;
	auto job = make_shared<Job>(nodeId, -1, -1, packet, time, JobType::PacketProcessing);
	Dispatcher::JobQueue.push(job);
}

void Dispatcher::QueuePacketUpload(int nodeId, int linkId, shared_ptr<Packet> packet, int delay)
{
	int time = CurrentTime + delay;
	cout << "[Dispatcher] Uploading Packet to execute at time " << time << endl;
	auto job = make_shared<Job>(nodeId, -1, linkId, packet, time, JobType::PacketUpload);
	Dispatcher::JobQueue.push(job);
}

void Dispatcher::QueuePacketDownload(int nodeId, int linkId, shared_ptr<Packet> packet, int delay)
{
	int time = CurrentTime + delay;
	cout << "[Dispatcher] Downloading Packet to execute at time " << time << endl;
	auto job = make_shared<Job>(nodeId, -1, linkId, packet, time, JobType::PacketDownload);
	Dispatcher::JobQueue.push(job);
}

int Dispatcher::GetCurrentTime()
{
	return Dispatcher::CurrentTime;
}

void Dispatcher::IncrementTime()
{
	Dispatcher::CurrentTime++;
}

vector<shared_ptr<Job>> Dispatcher::GetDueJobs()
{
	//cout << "GetDueJobs " << JobQueue.size() << endl;
	vector<shared_ptr<Job>> jobs;
	if (Dispatcher::JobQueue.empty())
		return jobs;
	
	auto job = Dispatcher::JobQueue.top();

	while (job->GetDelay() == Dispatcher::CurrentTime)
	{
		//cout << "found Job" << endl;
		jobs.push_back(job);
		Dispatcher::JobQueue.pop(); 
		job = Dispatcher::JobQueue.top();
	}

	return jobs;
}
