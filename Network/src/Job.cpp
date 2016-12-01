#include "Job.h"

using namespace std;

Job::Job()
{
	nodeId = -1;
	destId = -1;
	linkId = -1;
	packet = NULL;
	delay = -1;
	type = JobType::None;
}

Job::Job(int nid, int did, int lid, std::shared_ptr<Packet> p, int d, JobType jt)
{
	nodeId = nid;
	destId = did;
	linkId = lid;
	packet = p;
	delay = d;
	type = jt;
}

int Job::GetNodeId() const
{
	return nodeId;
}

int Job::GetDestId() const
{
	return destId;
}

int Job::GetLinkId() const
{
	return linkId;
}

shared_ptr<Packet> Job::GetPacket()
{
	return packet;
}

int Job::GetDelay() const
{
	return delay;
}

JobType Job::GetType() const
{
	return type;
}

void Job::SetNodeId(int nid)
{
	nodeId = nid;
}

void Job::SetDestId(int did)
{
	destId = did;
}

void Job::SetLinkId(int lid)
{
	linkId = lid;
}

void Job::SetPacket(shared_ptr<Packet> p)
{
	packet = p;
}

void Job::SetDelay(int d)
{
	delay = d;
}

void Job::SetJobType(JobType jt)
{
	type = jt;
}