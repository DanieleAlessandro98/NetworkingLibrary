#include "StdAfx.h"
#include "Client.h"
#include "ClientPacketHeaderMap.h"
#include <iostream>
#include <random>

using namespace Net;

Client::Client()
{
	isConnected = false;
	m_connectLimitTime = 0;
}

bool Client::Initialize(const char* c_szAddr, int port)
{
	try
	{
		if (!connectSocket.Create())
		{
			std::cerr << "Failed to create socket" << std::endl;
			return false;
		}

		CNetAddress netAddress;
		if (!netAddress.Set(c_szAddr, port))
		{
			std::cerr << "Failed to set address" << std::endl;
			return false;
		}

		u_long arg = 1;
		ioctlsocket(connectSocket.GetSocket(), FIONBIO, &arg);	// Non-blocking mode

		if (!connectSocket.Connect(netAddress))
		{
			std::cerr << "Failed to connect to the server" << std::endl;
			return false;
		}
	}
	catch (NetException ex)
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
		return false;
	}

	m_connectLimitTime = time(NULL) + 3;
	std::cout << "socket connected to the server" << std::endl;
	return true;
}

void Client::Process()
{
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(connectSocket.GetSocket(), &readfds);

	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(connectSocket.GetSocket(), &writefds);

	// Set timeout (optional)
	timeval timeout;
	timeout.tv_sec = 0; // 0 second timeout (immediatly)
	timeout.tv_usec = 0;

	int result = select(0, &readfds, &writefds, NULL, &timeout);
	if (result == SOCKET_ERROR)
	{
		std::cerr << "select failed with error: " << WSAGetLastError() << std::endl;
		return;
	}

	if (!isConnected)
	{
		if (FD_ISSET(connectSocket.GetSocket(), &writefds))
		{
			isConnected = true;
			std::cout << "OnConnectSuccess" << std::endl;
		}
		else if (time(NULL) > m_connectLimitTime)
		{
			std::cout << "OnConnectFailed" << std::endl;
		}

		return;
	}

	const auto dataStream = connectSocket.GetDataStream();
	if (!dataStream)
		return;

	if (FD_ISSET(connectSocket.GetSocket(), &writefds) && (dataStream->GetSendBufInputPos() > dataStream->GetSendBufOutpusPos()))
	{
		if (!dataStream->ProcessSend(connectSocket.GetSocket()))
		{
			int error = WSAGetLastError();

			if (error != WSAEWOULDBLOCK)
			{
				std::cout << "OnRemoteDisconnect" << std::endl;
				return;
			}
		}
	}

	if (FD_ISSET(connectSocket.GetSocket(), &readfds))
	{
		if (!dataStream->ProcessRecv(connectSocket.GetSocket()))
		{
			std::cout << "OnRemoteDisconnect" << std::endl;
			return;
		}
	}

	OnProcessRecv();
}

bool Client::IsConnected()
{
	return isConnected;
}

void Client::TestSend()
{
	const auto dataStream = connectSocket.GetDataStream();
	if (!dataStream)
		return;

	TPacketCGAction1 action1;
	action1.numIntero = 5;
	if (!dataStream->Send(sizeof(action1), &action1))
	{
		std::cerr << "action1 not sended" << std::endl;
		return;
	}

	std::cout << "action1 sended" << std::endl;
}

bool Client::TestRecv()
{
	const auto dataStream = connectSocket.GetDataStream();
	if (!dataStream)
		return false;

	TPacketGCServerResponse responsePacket;
	if (!dataStream->Recv(sizeof(responsePacket), &responsePacket))
		return false;

	TPacketGCServerResponseDynamic responseSubPacket;
	if (!dataStream->Recv(sizeof(responseSubPacket), &responseSubPacket))
		return false;

	std::cout << "response receved. message: " << responseSubPacket.response << "with num recv with dynamicpacket = " << responseSubPacket.num << std::endl;

	return true;
}

int Client::__GetRandNumber()
{
	std::random_device rd;  // Dispositivo di entropia per il seme iniziale
	std::mt19937 gen(rd()); // Generatore di numeri pseudo-casuali Mersenne Twister

	// Definisce la distribuzione per generare numeri interi tra 1 e 3 inclusi
	std::uniform_int_distribution<> dis(1, 3);

	// Genera un numero casuale
	return dis(gen);
}

void Client::TestSendAction3()
{
	const auto dataStream = connectSocket.GetDataStream();
	if (!dataStream)
		return;

	TPacketCGAction3WithSubH action3;
	action3.size = sizeof(action3);

	int random_number = __GetRandNumber();
	switch (random_number)
	{
		case 1:
		{
			action3.subheader = ACTION3_SUBHEADER_CG_1;

			TPacketCGAction3Sub1 sub;
			action3.size += sizeof(sub);
			sub.flag = true;

			if (!dataStream->Send(sizeof(action3), &action3))
			{
				std::cerr << "action3 not sended" << std::endl;
				return;
			}

			if (!dataStream->Send(sizeof(sub), &sub))
			{
				std::cerr << "action3-sub not sended" << std::endl;
				return;
			}

			std::cout << "action3 sended with sub = " << 1 << std::endl;
		}
		break;

		case 2:
		{
			action3.subheader = ACTION3_SUBHEADER_CG_2;

			TPacketCGAction3Sub2 sub;
			action3.size += sizeof(sub);
			sub.flag1 = true;
			sub.flag2 = true;

			if (!dataStream->Send(sizeof(action3), &action3))
			{
				std::cerr << "action3 not sended" << std::endl;
				return;
			}

			if (!dataStream->Send(sizeof(sub), &sub))
			{
				std::cerr << "action3-sub not sended" << std::endl;
				return;
			}

			std::cout << "action3 sended with sub = " << 2 << std::endl;
		}
		break;

		case 3:
		{
			action3.subheader = ACTION3_SUBHEADER_CG_3;

			TPacketCGAction3Sub3 sub;
			action3.size += sizeof(sub);
			sub.flag1 = true;
			sub.flag2 = true;
			sub.flag3 = true;

			if (!dataStream->Send(sizeof(action3), &action3))
			{
				std::cerr << "action3 not sended" << std::endl;
				return;
			}

			if (!dataStream->Send(sizeof(sub), &sub))
			{
				std::cerr << "action3-sub not sended" << std::endl;
				return;
			}

			std::cout << "action3 sended with sub = " << 3 << std::endl;
		}
		break;
	}
}

void Client::OnProcessRecv()
{
	TPacketHeader header = 0;

	bool ret = true;
	while (ret)
	{
		if (!CheckPacket(&header))
			break;

		switch (static_cast<Net::PacketGCHeader>(header))
		{
			case PacketGCHeader::HEADER_GC_SERVER_RESPONSE:
					ret = TestRecv();
					TestSendAction3();
					break;
		}
	}

	if (!ret)
		RecvErrorPacket(header);
}

bool Client::CheckPacket(Net::TPacketHeader* packetHeader)
{
	const auto dataStream = connectSocket.GetDataStream();
	if (!dataStream)
		return false;

	static CClientPacketHeaderMap packetHeaderMap;

	TPacketHeader tempHeader = 0;

	if (!dataStream->Peek(sizeof(TPacketHeader), &tempHeader))
		return false;

	if (0 == tempHeader)
	{
		if (!dataStream->Recv(sizeof(TPacketHeader), &tempHeader))
			return false;

		while (dataStream->Peek(sizeof(TPacketHeader), &tempHeader))
		{
			if (0 == tempHeader)
			{
				if (!dataStream->Recv(sizeof(TPacketHeader), &tempHeader))
					return false;
			}
			else
			{
				break;
			}
		}

		if (0 == tempHeader)
			return false;
	}

	CAbstractPacketHeaderMap::TPacketType packetType;
	if (!packetHeaderMap.Get(tempHeader, &packetType))
	{
		std::cerr << "Unknown packet header:" << std::endl;
		dataStream->ClearRecvBuffer();
		return false;
	}

	if (packetType.isDynamicSizePacket)
	{
		TDynamicSizePacketHeader dynamicSizePacketHeader;

		if (!dataStream->Peek(sizeof(TDynamicSizePacketHeader), &dynamicSizePacketHeader))
			return false;

		if (!dataStream->Peek(dynamicSizePacketHeader.size))
		{
			std::cerr << "Not enough dynamic packet size. Header: " << dynamicSizePacketHeader.header << " Packet size: " << dynamicSizePacketHeader.size << std::endl;
			return false;
		}
	}
	else
	{
		if (!dataStream->Peek(packetType.iPacketSize))
			return false;
	}

	if (!tempHeader)
		return false;

	*packetHeader = tempHeader;
	return true;
}

void Client::RecvErrorPacket(int header)
{
	const auto dataStream = connectSocket.GetDataStream();
	if (!dataStream)
		return;

	std::cerr << "Not handled this header: " << header << std::endl;
	dataStream->ClearRecvBuffer();
}
