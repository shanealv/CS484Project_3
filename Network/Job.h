#pragma once
#include <memory>

enum class JobType
{
	Processing,
	PacketUpload,
	PacketDownload,
	PacketCreation
};

class Job
{
private:
	int nodeId;
	int linkId;
	std::shared_ptr<Packet> packet;
	int delay;
	JobType job;
public:
	Job();
	Job(int nid, int lid, std::shared_ptr<Packet> p, double d, JobType jt);
	
	int GetNodeId();
	int GetLinkID();
	std::shared_ptr<Packet> getPacket();
	int GetDelay();
	JobType GetType();
	
	void SetNodeId(int nid);
	void SetLinkId(int lid);
	void SetPacket(std::shared_ptr<Packet> p);
	void SetDelay(int d);
	void SetJobType(JobType jt);
};