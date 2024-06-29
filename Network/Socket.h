#pragma once

#include <memory>
#include "Definition.h"
#include "NetException.h"
#include "NetAddress.h"
#include "DataStream.h"

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
			std::shared_ptr<CDataStream> GetDataStream() { return dataStream; }

		private:
			SOCKET m_Socket;
			std::shared_ptr<CDataStream> dataStream;
	};
}
