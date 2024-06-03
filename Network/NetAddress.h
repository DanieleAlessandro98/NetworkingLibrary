#pragma once

#include <string>
#include <winsock2.h>

namespace Net
{
	class CNetAddress
	{
		public:
			CNetAddress();
			~CNetAddress() = default;
			void Clear();

			bool Set(const char* c_szAddr, int port);
			void SetIP(const char* c_szIP);
			bool SetDNS(const char* c_szDNS);
			void SetPort(int port);

			std::string GetIP() const;

		protected:
			bool IsIP(const char* c_szAddr);

		private:
			SOCKADDR_IN m_sockAddrIn;
	};
}
