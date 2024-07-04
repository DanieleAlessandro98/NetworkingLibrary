#include "StdAfx.h"
#include "Server.h"
#include <iostream>
#include <Network/PacketDefinition.h>

using namespace Net;

Server::Server()
{
	m_packetManager = std::make_shared<CServerPacketManager>(this);
}

void Server::OnSocketListening()
{
	std::cout << "Socket Listening..." << std::endl;
}

void Server::OnConnectClient(std::shared_ptr<CSocket> client_data)
{
	if (!client_data)
		return;

	std::cout << "new client accepted" << std::endl;
}

void Server::OnDisconnectClient(std::shared_ptr<CSocket> client_data)
{
	if (!client_data)
		return;

	std::cout << "client disconnected" << std::endl;
}

bool Server::Analyze(TPacketHeader header, std::shared_ptr<CSocket> socket)
{
	bool ret = true;

	switch (static_cast<PacketCGHeader>(header))
	{
		case PacketCGHeader::HEADER_CG_ACTION1:
			ret = TestRecv(socket);
			TestSend(socket);
			break;

		case PacketCGHeader::HEADER_CG_ACTION_WITH_SUBH:
			ret = TestAction3Recv(socket);
			break;

		default:
			std::cerr << "Unknown packet header: " << static_cast<int>(header) << std::endl;
			ret = false;
			break;
	}

	return ret;
}

bool Server::TestRecv(std::shared_ptr<CSocket> clientSocket)
{
	if (!clientSocket)
		return false;

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
	if (!clientSocket)
		return false;

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
	if (!clientSocket)
		return false;

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
