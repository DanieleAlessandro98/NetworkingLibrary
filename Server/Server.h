#pragma once

#include "Network/Socket.h"
#include "Network/SocketWatcher.h"
#include <memory>

class Server
{
	public:
		bool Initialize(const char* c_szAddr, int port);
		void Process();
		void HandleNewConnection();

	private:
		Net::CSocket listenSocket;
		std::unique_ptr<Net::SocketWatcher> watcher;
};
