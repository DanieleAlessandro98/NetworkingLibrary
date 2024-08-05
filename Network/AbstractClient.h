#pragma once

#include "AbstractEntity.hpp"
#include "AbstractPacketHandler.h"
#include "AbstractPacketManager.h"

namespace Net
{
	class CAbstractClient : public AbstractEntity, public CAbstractPacketBaseHandler
	{
		public:
			CAbstractClient();
			~CAbstractClient() = default;

			bool Initialize(const char* c_szAddr, int port) override;
			void Process() override;
			void Shutdown() override;
			Net::CSocket* GetSocket() override { return connectSocket.get(); }

			void SetComponentsFactory(std::shared_ptr<AbstractClientComponentsFactory> factory);
			bool IsConnected();

			virtual void OnSocketCreated() = 0;
			virtual void OnConnect() = 0;
			virtual void OnConnectFail() = 0;
			virtual void OnDisconnect() = 0;

		protected:
			bool isConnected;
			time_t	m_connectLimitTime;

			std::shared_ptr<AbstractClientComponentsFactory> m_componentsFactory;
			std::shared_ptr<Net::CSocket> connectSocket;
			std::shared_ptr<CAbstractPacketManager> m_packetManager;
	};
}
