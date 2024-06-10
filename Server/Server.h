#pragma once

#include "Network/Socket.h"

class Server
{
	public:
		bool Initialize(const char* c_szAddr, int port);
		void Process();
		void HandleNewConnection(Net::CSocket newClientSocket);

	private:
		Net::CSocket listenSocket;
};
