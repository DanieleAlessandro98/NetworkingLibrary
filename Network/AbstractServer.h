#pragma once

#include "AbstractEntity.hpp"
#include "SocketWatcher.h"
#include "AbstractPacketManager.h"

namespace Net
{
	class CAbstractServer : public AbstractEntity
	{
		public:
			CAbstractServer() = default;
			~CAbstractServer() = default;

			bool Initialize(const char* c_szAddr, int port) override;
			void Process() override;
			void Shutdown() override;

			void HandleNewConnection();

			virtual void OnSocketListening() = 0;
			virtual bool CanAcceptNewConnection() = 0;
			virtual void OnConnectClient(std::shared_ptr<CSocket> newClientSocket) = 0;
			virtual void OnDisconnectClient(CAbstractPeer* peer) = 0;
			virtual void DisconnectAll() = 0;

		protected:
			CSocket listenSocket;
			std::shared_ptr<SocketWatcher> watcher;
			std::shared_ptr<CAbstractPacketManager> m_packetManager;
	};
}
