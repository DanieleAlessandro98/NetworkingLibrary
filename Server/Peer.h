#pragma once

#include <memory>
#include <Network/Socket.h>
#include <Network/SocketWatcher.h>
#include <Network/AbstractPeer.h>
#include <Network/AbstractPacketHandler.h>
#include "ServerHandshake.h"

class CPeer : public Net::CAbstractPeer
{
	public:
		CPeer(std::shared_ptr<Net::SocketWatcher> serverWatcher);
		~CPeer();

		void SetPhase(int phase) override;
		Net::CSocket* GetSocket() override { return m_socket.get(); }
		bool AnalyzePacket(Net::TPacketHeader header) override;

		void Setup(std::shared_ptr<Net::CSocket> socket, int handleCount, uint32_t handshake);
		void StartHandshake(uint32_t handshake);
		void SendHandshake(uint32_t curTime, long delta);
		bool HandshakeProcess(uint32_t time, long delta);

		void Packet(const void* c_pvData, int iSize);

		int GetHandle() const { return m_dwHandle; }
		uint32_t GetHandshake() const { return m_dwHandshake; }
		bool IsPhase(int phase) const { return m_iPhase == phase ? true : false; }

	private:
		bool			m_bDestroyed;
		int				m_iPhase;
		int								m_dwHandle;
		uint32_t						m_dwHandshake;
		std::shared_ptr<Net::CSocket>	m_socket;

		Net::CAbstractPacketServerHandler* m_packetHandler;
		std::unique_ptr<ServerHandshake> m_packetHandlerServerHandshake;
};
