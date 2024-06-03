#pragma once

#include "Definition.h"
#include "NetException.h"
#include "NetAddress.h"

namespace Net
{
	class CSocket
	{
		public:
			CSocket();
			CSocket(SOCKET socket);
			~CSocket() = default;

			bool Create();
			bool Close();
			bool Bind(CNetAddress address);
			bool Listen(CNetAddress address, int backlog = 5);
			bool Accept(CSocket& clientSocket);
			bool Connect(CNetAddress serverAddress);

			SOCKET GetSocket() { return m_Socket; }

		private:
			SOCKET m_Socket;
	};
}
