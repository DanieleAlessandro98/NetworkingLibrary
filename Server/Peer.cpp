#include "StdAfx.h"
#include "Peer.h"

CPeer::CPeer(std::shared_ptr<Net::SocketWatcher> serverWatcher)
{
	m_serverWatcher = serverWatcher;
	m_dwHandle = 0;
}

void CPeer::Setup(std::shared_ptr<Net::CSocket> socket, int handleCount, uint32_t handshake)
{
	m_socket = socket;
	m_dwHandle = handleCount;

	AddSocketToWatcher(m_socket->GetSocket(), this);

	StartHandshake(handshake);
}

void CPeer::StartHandshake(uint32_t handshake)
{
	m_dwHandshake = handshake;
}
