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
			if (FDW_READ == watcher->has_event(listenSocket.GetSocket(), event_idx))
			{
				HandleNewConnection();
				watcher->clear_event(listenSocket.GetSocket(), event_idx);
			}

			continue;
		}

		const auto dataStream = d->GetDataStream();
		if (!dataStream)
			continue;

		int iRet = watcher->has_event(d->GetSocket(), event_idx);
		switch (iRet)
		{
			case FDW_READ:
			{
				if (!dataStream->ProcessRecv(d->GetSocket()))
					std::cerr << "SetPhase(PHASE_CLOSE)" << std::endl;
			}
			break;

			case FDW_WRITE:
			{
				if (!dataStream->ProcessSend(d->GetSocket()))
					std::cerr << "SetPhase(PHASE_CLOSE)" << std::endl;
			}
			break;

			case FDW_EOF:
			{
				std::cerr << "SetPhase(PHASE_CLOSE)" << std::endl;
			}
			break;

			default:
				printf("watcher->has_event returned unknown %d", iRet);
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

//while (true)
//{
//	PacketHeader header;
//	if (!newClientSocket.Recv(header))
//		break;
//
//	switch (header)
//	{
//	case PacketHeader::HEADER_ACTION1:
//	{
//		TPacketAction1 action;
//		if (!newClientSocket.Recv(action))
//			break;
//
//		std::cout << "Receved: " << "HEADER_ACTION1" << std::endl;
//		std::cout << "numIntero: " << action.numIntero << std::endl;
//	}
//	break;
//
//	case PacketHeader::HEADER_ACTION2:
//	{
//		// ...
//	}
//	break;
//	}
//}
//
//if (listenSocket.Close())
//{
//	std::cout << "socket closed" << std::endl;
//}
//else
//{
//	std::cerr << "Failed to close socket" << std::endl;
//}
