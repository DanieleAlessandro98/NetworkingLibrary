#include "StdAfx.h"
#include "Socket.h"

namespace Net
{
	CSocket::CSocket()
	{
		this->m_Socket = INVALID_SOCKET;
	}

	CSocket::CSocket(SOCKET socket)
	{
		this->m_Socket = socket;
	}

	bool CSocket::Create()
	{
		if (m_Socket != INVALID_SOCKET)
			return false;

		m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == INVALID_SOCKET)
			throw NetException("CSocket::Create - Error at socket(): %ld\n", WSAGetLastError());

		return true;
	}

	bool CSocket::Close()
	{
		if (m_Socket == INVALID_SOCKET)
			return false;

		if (closesocket(m_Socket) != 0)
			throw NetException("CSocket::Close - Error at closesocket(): %ld\n", WSAGetLastError());

		return true;
	}

	bool CSocket::Bind(CNetAddress address)
	{
		SOCKADDR_IN addrIn = address.GetAddrIn();
		if (bind(m_Socket, (SOCKADDR*)&addrIn, sizeof(addrIn)) != 0)
			throw NetException("CSocket::Bind - Error at bind(): %ld\n", WSAGetLastError());

		return true;
	}

	bool CSocket::Listen(CNetAddress address, int backlog)
	{
		if (!Bind(address))
			return false;

		if (listen(m_Socket, backlog) != 0)
			throw NetException("CSocket::Listen - Error at listen(): %ld\n", WSAGetLastError());

		return true;
	}

	bool CSocket::Accept(CSocket& clientSocket)
	{
		sockaddr_in addr = {};
		int len = sizeof(sockaddr_in);

		SOCKET acceptSocket = accept(m_Socket, (sockaddr*)(&addr), &len);
		if (acceptSocket == INVALID_SOCKET)
			throw NetException("CSocket::Accept - Error at accept(): %ld\n", WSAGetLastError());

		clientSocket = CSocket(acceptSocket);

		return true;
	}

	bool CSocket::Connect(CNetAddress serverAddress)
	{
		SOCKADDR_IN addrIn = serverAddress.GetAddrIn();
		if (connect(m_Socket, (SOCKADDR*)&addrIn, sizeof(addrIn)) != 0)
			throw NetException("CSocket::Connect - Error at connect(): %ld\n", WSAGetLastError());

		return true;
	}

	bool CSocket::Send(const void* buf, const int bytesToSend, int& partialBytesSent)
	{
		partialBytesSent = send(m_Socket, static_cast<const char*>(buf), bytesToSend, 0);
		if (partialBytesSent == SOCKET_ERROR)
			return false;

		return true;
	}

	bool CSocket::Recv(void* buf, int len, int& partialBytesRecv)
	{
		partialBytesRecv = recv(m_Socket, static_cast<char*>(buf), len, 0);
		if (partialBytesRecv == SOCKET_ERROR)
			return false;

		return true;
	}

	bool CSocket::SendAll(const void* buf, int len)
	{
		int totalBytesSent = 0;

		while (totalBytesSent < len)
		{
			const void* bufToSend = static_cast<const char*>(buf) + totalBytesSent;
			int bytesToSend = len - totalBytesSent;
			int partialBytesSent;

			if (!Send(bufToSend, bytesToSend, partialBytesSent))
				return false;

			totalBytesSent += partialBytesSent;
		}

		return true;
	}

	bool CSocket::RecvAll(void* buf, int len)
	{
		int totalBytesRecv = 0;

		while (totalBytesRecv < len)
		{
			void* bufToRecv = static_cast<char*>(buf) + totalBytesRecv;
			int bytesToRecv = len - totalBytesRecv;
			int partialBytesRecv;

			if (!Recv(bufToRecv, bytesToRecv, partialBytesRecv))
				return false;

			totalBytesRecv += partialBytesRecv;
		}

		return true;
	}

	bool CSocket::Send(const CPacket& packet)
	{
		uint32_t packetSize = htonl(packet.GetSize());
		if (!SendAll(&packetSize, sizeof(packetSize)))
			return false;

		return SendAll(packet.GetData(), packet.GetSize());
	}

	bool CSocket::Recv(CPacket& packet)
	{
		uint32_t packetSize;
		if (!RecvAll(&packetSize, sizeof(packetSize)))
			return false;

		packetSize = ntohl(packetSize);
		packet.ResizeBuffer(packetSize);
		return RecvAll(packet.BeginBuffer(), packetSize);
	}

	bool CSocket::Send(const TPacketAction1& action1)
	{
		uint16_t header = htons(static_cast<uint16_t>(action1.header));
		if (!SendAll(&header, sizeof(header)))
			return false;

		CPacket packet;
		packet << action1.numIntero;
		return Send(packet);
	}

	bool CSocket::Recv(PacketHeader& header)
	{
		uint16_t headerRecv;
		if (!RecvAll(&headerRecv, sizeof(headerRecv)))
			return false;

		headerRecv = ntohs(headerRecv);
		header = static_cast<PacketHeader>(headerRecv);

		return true;
	}

	bool CSocket::Recv(TPacketAction1& action1)
	{
		CPacket packet;
		if (!Recv(packet))
			return false;

		packet >> action1.numIntero;
		return true;
	}
}
