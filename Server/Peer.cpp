#include "StdAfx.h"
#include "Peer.h"
#include <cassert>

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

CPeer::CPeer(std::shared_ptr<Net::SocketWatcher> serverWatcher)
{
	m_serverWatcher = serverWatcher;
	m_dwHandle = 0;
	m_bDestroyed = false;
	m_iPhase = PHASE_CLOSE;

	m_packetHandler = nullptr;
	m_packetHandlerServerHandshake = std::make_unique<ServerHandshake>();
}

CPeer::~CPeer()
{
	if (m_bDestroyed)
		return;

	m_dwHandle = 0;
	m_bDestroyed = true;

	if (m_socket != nullptr)
	{
		RemoveSocketToWatcher(m_socket->GetSocket());
		m_socket->Close();
		m_socket.reset();
	}
}

void CPeer::SetPhase(int phase)
{
	m_iPhase = phase;

	switch (m_iPhase)
	{
		case PHASE_CLOSE:
			m_packetHandler = nullptr;
			break;

		case PHASE_HANDSHAKE:
			m_packetHandler = m_packetHandlerServerHandshake.get();
			break;
	}
}

bool CPeer::AnalyzePacket(Net::TPacketHeader header)
{
	if (m_iPhase == PHASE_CLOSE)
		return false;

	return m_packetHandler->Analyze(this, header);
}

void CPeer::Setup(std::shared_ptr<Net::CSocket> socket, int handleCount, uint32_t handshake)
{
	m_socket = socket;
	m_dwHandle = handleCount;

	AddSocketToWatcher(m_socket->GetSocket());

	SetPhase(PHASE_HANDSHAKE);
	StartHandshake(handshake);
}

void CPeer::StartHandshake(uint32_t handshake)
{
	m_dwHandshake = handshake;
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

void CPeer::Packet(const void* c_pvData, int iSize)
{
	assert(iSize > 0);

	if (m_iPhase == PHASE_CLOSE)
		return;

	if (!m_socket)
		return;

	const auto dataStream = m_socket->GetDataStream();
	if (!dataStream)
		return;

	if (!dataStream->Send(iSize, c_pvData))
		return;

	m_serverWatcher->add_fd(m_socket->GetSocket(), this, Net::EFdwatch::FDW_WRITE, true);
}
