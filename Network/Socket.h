#pragma once

#include "Definition.h"
#include "NetException.h"

namespace Net
{
	class CSocket
	{
		public:
			CSocket();
			~CSocket() = default;

			bool Create();
			bool Close();

			SOCKET GetSocket() { return m_Socket; }

		private:
			SOCKET m_Socket;
	};
}
