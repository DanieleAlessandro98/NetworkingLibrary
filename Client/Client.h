#pragma once

#include "Network/Socket.h"
#include <Network/DataStream.h>
#include <Network/PacketDefinition.h>
#include <memory>

class Client
{
	public:
		Client();
		~Client() = default;

		bool Initialize(const char* c_szAddr, int port);
		void Process();
		bool IsConnected();

		void OnProcessRecv();
		bool CheckPacket(Net::TPacketHeader* header);
		void RecvErrorPacket(int header);

		void TestSend();
		bool TestRecv();

	private:
		Net::CSocket connectSocket;
		bool isConnected;
		time_t	m_connectLimitTime;
		std::unique_ptr<Net::CDataStream> dataStream;
};
