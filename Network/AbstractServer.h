#pragma once

#include "AbstractEntity.h"
#include "SocketWatcher.h"
#include "AbstractServerPacketManager.h"

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
			virtual void OnDisconnectClient(std::shared_ptr<CSocket> client_data) = 0;
			virtual void DisconnectAll() = 0;

		protected:
			CSocket listenSocket;
			std::shared_ptr<SocketWatcher> watcher;
			std::shared_ptr<CAbstractServerPacketManager> m_packetManager;
	};
}
