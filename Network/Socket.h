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
			~CSocket() = default;

			bool Create();
			bool Close();
			bool Bind(CNetAddress address);
			bool Listen(CNetAddress address, int backlog = 5);

			SOCKET GetSocket() { return m_Socket; }

		private:
			SOCKET m_Socket;
	};
}
