#include "StdAfx.h"
#include "AbstractPeer.h"
#include <cassert>
#include "PacketIO.hpp"

namespace Net
{
	CAbstractPeer::CAbstractPeer(std::shared_ptr<SocketWatcher> serverWatcher)
	{
		m_serverWatcher = serverWatcher;
		m_dwHandle = 0;
		m_bDestroyed = false;
		m_iPhase = PHASE_CLOSE;

		m_packetHandler = nullptr;
		RegisterPhaseHandler(PHASE_CLOSE, nullptr);
	}

	CAbstractPeer::~CAbstractPeer()
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

	void CAbstractPeer::RegisterPhaseHandler(int phase, CAbstractPacketServerHandler* handler)
	{
		m_phaseHandlers[phase] = handler;
	}

	void CAbstractPeer::SetPhase(int phase)
	{
		m_iPhase = phase;

		if (m_phaseHandlers.find(phase) != m_phaseHandlers.end())
			m_packetHandler = m_phaseHandlers[phase];
		else
			m_packetHandler = nullptr;
	}

	void CAbstractPeer::AddSocketToWatcher(int fd)
	{
		m_serverWatcher->add_fd(fd, this, EFdwatch::FDW_READ, false);
	}

	void CAbstractPeer::RemoveSocketToWatcher(int fd)
	{
		m_serverWatcher->remove_fd(fd);
	}

	void CAbstractPeer::Setup(std::shared_ptr<CSocket> socket, int handleCount, uint32_t handshake)
	{
		m_socket = socket;
		m_dwHandle = handleCount;
		m_dwHandshake = handshake;

		AddSocketToWatcher(m_socket->GetSocket());

		OnSetupCompleted();
	}

	void CAbstractPeer::Packet(const void* c_pvData, int iSize)
	{
		assert(iSize > 0);

		if (m_iPhase == PHASE_CLOSE)
			return;

		if (!CPacketIO::WritePacketData(GetSocket(), c_pvData, iSize))
			return;

		m_serverWatcher->add_fd(m_socket->GetSocket(), this, EFdwatch::FDW_WRITE, true);
	}

	bool CAbstractPeer::AnalyzePacket(TPacketHeader header)
	{
		if (m_iPhase == PHASE_CLOSE)
			return false;

		return m_packetHandler->Analyze(this, header);
	}
}
