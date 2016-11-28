#pragma once
#include <memory>
#include "Packet.h"
#include <functional>

enum class JobType
{
	None,
	PacketProcessing,
	PacketUpload,
	PacketDownload,
	PacketCreation
};

class Job
{
private:
	int nodeId;
	int destId;
	int linkId;
	std::shared_ptr<Packet> packet;
	int delay;
	JobType type;
public:
	Job();
	Job(int nid, int did, int lid, std::shared_ptr<Packet> p, int d, JobType jt);
	
	int GetNodeId() const;
	int GetDestId() const;
	int GetLinkID() const;
	std::shared_ptr<Packet> getPacket() const;
	int GetDelay() const;
	JobType GetType() const;
	
	void SetNodeId(int nid);
	void SetDestId(int did);
	void SetLinkId(int lid);
	void SetPacket(std::shared_ptr<Packet> p);
	void SetDelay(int d);
	void SetJobType(JobType jt);
};

struct DelayComparison : public std::binary_function<Job&, Job&, bool>
{
    bool operator() (const Job &j1, const Job &j2) const
    {
        return j1.GetDelay() > j2.GetDelay();
    }
};