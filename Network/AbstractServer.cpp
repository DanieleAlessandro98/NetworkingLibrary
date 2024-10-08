#include "StdAfx.h"
#include <iostream>
#include "AbstractServer.h"
#include "AbstractPeer.h"

namespace Net
{
	bool CAbstractServer::Initialize(const char* c_szAddr, int port)
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

			if (!listenSocket.Listen(netAddress))
			{
				std::cerr << "Failed to listen socket" << std::endl;
				return false;
			}

			if (m_componentsFactory)
			{
				m_packetManager = m_componentsFactory->CreatePacketManager();
				m_packetManager->Initialize();

				m_peerManager = m_componentsFactory->CreatePeerManager();
			}
			else
			{
				std::cerr << "ComponentsFactory not set" << std::endl;
				return false;
			}
		}
		catch (NetException ex)
		{
			std::cerr << "Exception: " << ex.what() << std::endl;
			return false;
		}

		watcher = std::make_unique<SocketWatcher>(4096);
		watcher->add_socket(listenSocket.GetSocket(), NULL, SOCKW_READ, false);

		OnInitializeCompleted();
		OnSocketListening();
		return true;
	}

	void CAbstractServer::Process()
	{
		m_peerManager->DestroyClosed();

		int num_events = watcher->monitor(0);
		if (num_events < 0)
			return;

		CAbstractPeer* d;

		for (int event_idx = 0; event_idx < num_events; ++event_idx)
		{
			d = watcher->get_client_data(event_idx);
			if (!d)
			{
				if (SOCKW_READ == watcher->get_event_status(listenSocket.GetSocket(), event_idx))
				{
					HandleNewConnection();
					watcher->clear_event(listenSocket.GetSocket(), event_idx);
				}

				continue;
			}

			const auto peerSocket = d->GetSocket();
			if (!peerSocket)
				continue;

			const auto dataStream = peerSocket->GetDataStream();
			if (!dataStream)
				continue;

			int iRet = watcher->get_event_status(peerSocket->GetSocket(), event_idx);
			switch (iRet)
			{
				case SOCKW_READ:
				{
					if (!m_packetManager->ProcessRecv(d))
					{
						d->SetPhase(PHASE_CLOSE);
						OnDisconnectClient(d);
					}
				}
				break;

				case SOCKW_WRITE:
				{
					if (!dataStream->ProcessSend(peerSocket->GetSocket()))
					{
						d->SetPhase(PHASE_CLOSE);
						OnDisconnectClient(d);
					}
				}
				break;

				case SOCKW_EOF:
				{
					d->SetPhase(PHASE_CLOSE);
				}
				break;

				default:
					printf("watcher->get_event_status returned unknown %d", iRet);
					d->SetPhase(PHASE_CLOSE);
					break;
			}
		}
	}

	void CAbstractServer::SetComponentsFactory(std::shared_ptr<AbstractServerComponentsFactory> factory)
	{
		m_componentsFactory = factory;
	}

	void CAbstractServer::HandleNewConnection()
	{
		auto newClientSocket = std::make_shared<CSocket>();
		if (!listenSocket.Accept(*newClientSocket))
		{
			std::cerr << "Failed to accept new client" << std::endl;
			newClientSocket->Close();
			return;
		}

		if (!m_peerManager->CanAcceptNewConnection())
		{
			std::cerr << "max connection reached. MAX_ALLOW_USER = %d" << SERVER_MAX_INCOMING_CONNECTIONS << std::endl;
			newClientSocket->Close();
			return;
		}

		m_peerManager->AcceptPeer(m_componentsFactory.get(), newClientSocket, watcher);
		OnConnectClient(newClientSocket);
	}

	void CAbstractServer::Shutdown()
	{
		DisconnectAll();

		watcher->remove_socket(listenSocket.GetSocket());
		listenSocket.Close();
	}
}
