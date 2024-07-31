#pragma once

#include <memory>
#include <set>
#include <unordered_map>
#include <Network/Socket.h>
#include <Network/SocketWatcher.h>
#include "Peer.h"

class CPeerManager
{
	public:
		CPeerManager();
		~CPeerManager();

		void DisconnectAll();
		void DestroyDesc(CPeer* d, bool skipMapErase = false);

		bool CanAcceptNewConnection();
		void AcceptPeer(std::shared_ptr<Net::CSocket> socket, std::shared_ptr<Net::SocketWatcher> serverWatcher);

		std::shared_ptr<CPeer> GetFirstPeer();

	protected:
		uint32_t __CreateHandshake();

	private:
		int				m_iPeerConnected;
		int				m_iHandleCount;
		bool			m_bDestroyed;
		std::set<uint32_t> m_setHandshake;
		std::unordered_map<uint32_t, std::shared_ptr<CPeer>> m_mapPeer;
};
