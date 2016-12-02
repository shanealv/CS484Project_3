#include "Job.h"
#include "Packet.h"
#include <iostream>
#include <memory>

using namespace std;

Job::Job(int nid, int did, int lid, std::shared_ptr<Packet> p, int d, JobType jt)
{
	nodeId = nid;
	destId = did;
	linkId = lid;
	packet = p;
	delay = d;
	type = jt;
}

int Job::GetNodeId()
{
	return nodeId;
}

int Job::GetDestId()
{
	return destId;
}

int Job::GetLinkId()
{
	return linkId;
}

shared_ptr<Packet> Job::GetPacket()
{
	return packet;
}

int Job::GetDelay()
{
	return delay;
}

JobType Job::GetType()
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

void Job::SetPacket(shared_ptr<Packet>& p)
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
