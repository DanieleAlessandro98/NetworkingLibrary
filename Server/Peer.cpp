#include "StdAfx.h"
#include "Peer.h"

void gettimeofday(struct timeval* t, struct timezone* dummy)
{
	uint32_t millisec = GetTickCount();

	t->tv_sec = (millisec / 1000);
	t->tv_usec = (millisec % 1000) * 1000;
}

static uint32_t get_boot_sec()
{
	static struct timeval tv_boot = { 0L, 0L };

	if (tv_boot.tv_sec == 0)
		gettimeofday(&tv_boot, NULL);

	return tv_boot.tv_sec;
}

uint32_t get_dword_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	//tv.tv_sec -= 1057699978;
	tv.tv_sec -= get_boot_sec();
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

CPeer::CPeer(std::shared_ptr<Net::SocketWatcher> serverWatcher) :
	Net::CAbstractPeer(serverWatcher)
{
	m_packetHandlerServerHandshake = std::make_unique<ServerHandshake>();
	RegisterPhaseHandler(PHASE_HANDSHAKE, m_packetHandlerServerHandshake.get());
}

void CPeer::OnSetupCompleted()
{
	SetPhase(PHASE_HANDSHAKE);
	StartHandshake();
}

void CPeer::StartHandshake()
{
	SendHandshake(get_dword_time(), 0);
}

void CPeer::SendHandshake(uint32_t curTime, long delta)
{
	Net::TPacketGCHandshake pack;

	pack.handshake = m_dwHandshake;
	pack.time = curTime;
	pack.delta = delta;

	Packet(&pack, sizeof(Net::TPacketGCHandshake));
}

bool CPeer::HandshakeProcess(uint32_t time, long delta)
{
	// Simulation of the handshake process. not concerned now
	return true;
}
