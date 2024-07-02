#include "StdAfx.h"
#include "Server.h"
#include "ServerPacketHeaderMap.h"
#include <iostream>
#include <Network/PacketDefinition.h>

using namespace Net;

bool Server::Initialize(const char* c_szAddr, int port)
{
	try
	{
		if (!listenSocket.Create())
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

		unsigned long val = 1;
		ioctlsocket(listenSocket.GetSocket(), FIONBIO, &val);

		if (!listenSocket.Listen(netAddress))
		{
			std::cerr << "Failed to listen socket" << std::endl;
			return false;
		}
	}
	catch (NetException ex)
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
		return false;
	}

	watcher = std::make_unique<SocketWatcher>(4096);
	watcher->add_fd(listenSocket.GetSocket(), NULL, FDW_READ, false);

	std::cout << "Socket Listening..." << std::endl;
	return true;
}

void Server::Process()
{
	int num_events = watcher->monitor(0);
	if (num_events < 0)
		return;

	std::shared_ptr<CSocket> d;

	for (int event_idx = 0; event_idx < num_events; ++event_idx)
	{
		d = watcher->get_client_data(event_idx);
		if (!d)
		{
			if (FDW_READ == watcher->get_event_status(listenSocket.GetSocket(), event_idx))
			{
				HandleNewConnection();
				watcher->clear_event(listenSocket.GetSocket(), event_idx);
			}

			continue;
		}

		const auto dataStream = d->GetDataStream();
		if (!dataStream)
			continue;

		int iRet = watcher->get_event_status(d->GetSocket(), event_idx);
		switch (iRet)
		{
			case FDW_READ:
			{
				if (!ProcessRecv(d))
					std::cerr << "SetPhase(PHASE_CLOSE)" << std::endl;
			}
			break;

			case FDW_WRITE:
			{
				if (!dataStream->ProcessSend(d->GetSocket()))
				{
					std::cerr << "Client disconnected" << std::endl;
					watcher->remove_fd(d->GetSocket());
				}
			}
			break;

			case FDW_EOF:
			{
				std::cerr << "SetPhase(PHASE_CLOSE)" << std::endl;
			}
			break;

			default:
				printf("watcher->get_event_status returned unknown %d", iRet);
				break;
		}
	}
}

void Server::HandleNewConnection()
{
	auto newClientSocket = std::make_shared<CSocket>();
	if (!listenSocket.Accept(*newClientSocket))
	{
		std::cerr << "Failed to accept new client" << std::endl;
		newClientSocket->Close();
		return;
	}

	watcher->add_fd(newClientSocket->GetSocket(), newClientSocket, FDW_READ, false);
	std::cout << "new client accepted" << std::endl;
}

bool Server::ProcessRecv(std::shared_ptr<CSocket> clientSocket)
{
	const auto dataStream = clientSocket->GetDataStream();
	if (!dataStream)
		return false;

	if (!dataStream->ProcessRecv(clientSocket->GetSocket()))
	{
		std::cout << "Client disconnected" << std::endl;
		watcher->remove_fd(clientSocket->GetSocket());
		return false;
	}

	return OnProcessRecv(clientSocket);
}

bool Server::OnProcessRecv(std::shared_ptr<CSocket> clientSocket)
{
	TPacketHeader header = 0;

	bool ret = true;
	while (ret)
	{
		if (!CheckPacket(clientSocket, &header))
			break;

		switch (static_cast<Net::PacketCGHeader>(header))
		{
			case PacketCGHeader::HEADER_CG_ACTION1:
				ret = TestRecv(clientSocket);
				TestSend(clientSocket);
				break;

			case PacketCGHeader::HEADER_CG_ACTION_WITH_SUBH:
				ret = TestAction3Recv(clientSocket);
				break;

			default:
				std::cerr << "Unknown packet header: " << header << std::endl;
				ret = false;
				break;
		}
	}

	if (!ret)
		RecvErrorPacket(clientSocket, header);

	return ret;
}

bool Server::TestRecv(std::shared_ptr<CSocket> clientSocket)
{
	const auto dataStream = clientSocket->GetDataStream();
	if (!dataStream)
		return false;

	TPacketCGAction1 action1Packet;
	if (!dataStream->Recv(sizeof(action1Packet), &action1Packet))
		return false;

	std::cout << "action1 receved. numIntero = " << action1Packet.numIntero << std::endl;
	return true;
}

bool Server::TestSend(std::shared_ptr<CSocket> clientSocket)
{
	const auto dataStream = clientSocket->GetDataStream();
	if (!dataStream)
		return false;

	TPacketGCServerResponse responsePacket;
	responsePacket.size = sizeof(responsePacket);

	TPacketGCServerResponseDynamic responseSubPacket;
	responsePacket.size += sizeof(responseSubPacket);

	strncpy(responseSubPacket.response, "blabla message server blabla", sizeof(responseSubPacket.response) - 1);
	responseSubPacket.response[sizeof(responseSubPacket.response) - 1] = '\0';
	responseSubPacket.num = 33;

	if (!dataStream->Send(sizeof(responsePacket), &responsePacket))
		return false;

	if (!dataStream->Send(sizeof(responseSubPacket), &responseSubPacket))
		return false;

	std::cout << "Response sended." << std::endl;

	watcher->add_fd(clientSocket->GetSocket(), clientSocket, FDW_WRITE, true);
	return true;
}

bool Server::TestAction3Recv(std::shared_ptr<CSocket> clientSocket)
{
	const auto dataStream = clientSocket->GetDataStream();
	if (!dataStream)
		return false;

	TPacketCGAction3WithSubH action3Packet;
	if (!dataStream->Recv(sizeof(action3Packet), &action3Packet))
		return false;

	switch (action3Packet.subheader)
	{
		case ACTION3_SUBHEADER_CG_1:
		{
			TPacketCGAction3Sub1 sub;
			if (!dataStream->Recv(sizeof(sub), &sub))
				return false;

			std::cout << "action3 receved. sub = " << 1 << "flag = " << sub.flag << std::endl;
		}
		break;

		case ACTION3_SUBHEADER_CG_2:
		{
			TPacketCGAction3Sub2 sub;
			if (!dataStream->Recv(sizeof(sub), &sub))
				return false;

			std::cout << "action3 receved. sub = " << 2 << "flag = " << sub.flag1 << sub.flag2 << std::endl;
		}
		break;

		case ACTION3_SUBHEADER_CG_3:
		{
			TPacketCGAction3Sub3 sub;
			if (!dataStream->Recv(sizeof(sub), &sub))
				return false;

			std::cout << "action3 receved. sub = " << 3 << "flag = " << sub.flag1 << sub.flag2 << sub.flag3 << std::endl;
		}
		break;
	}

	return true;
}

bool Server::CheckPacket(std::shared_ptr<CSocket> clientSocket, TPacketHeader* packetHeader)
{
	const auto dataStream = clientSocket->GetDataStream();
	if (!dataStream)
		return false;

	TPacketHeader tempHeader = 0;

	static CServerPacketHeaderMap packetHeaderMap;

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

void Server::RecvErrorPacket(std::shared_ptr<CSocket> clientSocket, int header)
{
	const auto dataStream = clientSocket->GetDataStream();
	if (!dataStream)
		return;

	std::cerr << "Not handled this header: " << header << std::endl;
	dataStream->ClearRecvBuffer();
}
