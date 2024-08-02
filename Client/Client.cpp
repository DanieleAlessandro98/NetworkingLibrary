#include "StdAfx.h"
#include "Client.h"
#include <iostream>
#include <random>

using namespace Net;

Client::Client()
{
	m_packetManager = std::make_shared<CClientPacketManager>();
}

void Client::OnSocketCreated()
{
	std::cout << "socket created" << std::endl;
}

void Client::OnConnect()
{
	std::cout << "socket connected to the server" << std::endl;
}

void Client::OnConnectFail()
{
	std::cout << "OnConnectFailed" << std::endl;
}

void Client::OnDisconnect()
{
	std::cout << "OnRemoteDisconnect" << std::endl;
}

bool Client::AnalyzePacket(Net::TPacketHeader header)
{
	bool ret = true;

	switch (static_cast<Net::PacketGCHeader>(header))
	{
		case PacketGCHeader::HEADER_GC_HANDSHAKE:
			ret = RecvHandshake();
			break;

		default:
			std::cerr << "Unknown packet header: " << static_cast<int>(header) << std::endl;
			ret = false;
			break;
	}

	return ret;
}

bool Client::RecvHandshake()
{
	if (!connectSocket)
		return false;

	const auto dataStream = connectSocket->GetDataStream();
	if (!dataStream)
		return false;

	TPacketGCHandshake handshakePacket;
	if (!dataStream->Recv(sizeof(handshakePacket), &handshakePacket))
		return false;

	std::cout << "Handshake Recv:" << handshakePacket.time << "\t" << handshakePacket.delta << std::endl;

	SendHandshake(handshakePacket.handshake, handshakePacket.time, handshakePacket.delta);

	return true;
}

void Client::SendHandshake(uint32_t handshake, uint32_t time, long delta)
{
	if (!connectSocket)
		return;

	const auto dataStream = connectSocket->GetDataStream();
	if (!dataStream)
		return;

	TPacketCGHandshake packet;

	packet.handshake = handshake;
	packet.time = time;
	packet.delta = delta;

	if (!dataStream->Send(sizeof(packet), &packet))
	{
		std::cerr << "Handshake not sended" << std::endl;
		return;
	}

	std::cout << "Handshake Send" << std::endl;
}
