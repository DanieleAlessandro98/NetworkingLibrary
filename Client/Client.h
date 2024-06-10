#pragma once

#include "Network/Socket.h"

class Client
{
	public:
		Client();
		~Client() = default;

		bool Initialize(const char* c_szAddr, int port);
		void Process();
		bool IsConnected();

	private:
		Net::CSocket connectSocket;
		bool isConnected;
		time_t	m_connectLimitTime;
};
