#include "StdAfx.h"
#include "Server.h"
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

		switch (static_cast<Net::PacketHeader>(header))
		{
			case PacketHeader::HEADER_ACTION1:
				ret = TestRecv(clientSocket);
				TestSend(clientSocket);
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

	TPacketAction1 action1Packet;
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

	TPacketAction1 action1Packet;
	action1Packet.numIntero = 22;
	if (!dataStream->Send(sizeof(action1Packet), &action1Packet))
		return false;

	std::cout << "action1 sended." << std::endl;

	watcher->add_fd(clientSocket->GetSocket(), clientSocket, FDW_WRITE, true);
	return true;
}

bool Server::CheckPacket(std::shared_ptr<CSocket> clientSocket, TPacketHeader* packetHeader)
{
	const auto dataStream = clientSocket->GetDataStream();
	if (!dataStream)
		return false;

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

	if (!dataStream->Peek(sizeof(TPacketAction1)))
		return false;

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
