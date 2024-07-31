#pragma once

#include <memory>
#include <Network/Socket.h>
#include <Network/SocketWatcher.h>
#include <Network/AbstractPeer.h>

class CPeer : Net::CAbstractPeer
{
	public:
		CPeer(std::shared_ptr<Net::SocketWatcher> serverWatcher);
		~CPeer();

		void Setup(std::shared_ptr<Net::CSocket> socket, int handleCount, uint32_t handshake);
		void StartHandshake(uint32_t handshake);

		Net::CSocket* GetSocket() override { return m_socket.get(); }
		int GetHandle() const { return m_dwHandle; }
		uint32_t GetHandshake() const { return m_dwHandshake; }

	private:
		bool			m_bDestroyed;

		int								m_dwHandle;
		uint32_t						m_dwHandshake;
		std::shared_ptr<Net::CSocket>	m_socket;
};
