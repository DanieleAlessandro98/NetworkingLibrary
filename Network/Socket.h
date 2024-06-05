#pragma once

#include "Definition.h"
#include "NetException.h"
#include "NetAddress.h"
#include "Packet.h"

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
			bool Send(const void* buf, const int bytesToSend, int& partialBytesSent);
			bool Recv(void* buf, int len, int& partialBytesRecv);
			bool SendAll(const void* buf, int len);
			bool RecvAll(void* buf, int len);

			bool Send(const CPacket& packet);
			bool Recv(CPacket& packet);

			SOCKET GetSocket() { return m_Socket; }

		private:
			SOCKET m_Socket;
	};
}
