#include "StdAfx.h"
#include "Peer.h"

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

void CPeer::Setup(std::shared_ptr<Net::CSocket> socket, int handleCount, uint32_t handshake)
{
	m_socket = socket;
	m_dwHandle = handleCount;

	AddSocketToWatcher(m_socket->GetSocket());

	StartHandshake(handshake);
}

void CPeer::StartHandshake(uint32_t handshake)
{
	m_dwHandshake = handshake;
}
