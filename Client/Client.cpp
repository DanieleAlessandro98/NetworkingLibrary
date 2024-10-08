#include "StdAfx.h"
#include "Client.h"
#include <iostream>
#include <random>
#include "ClientComponentsFactory.hpp"
#include "Network/PacketIO.hpp"

using namespace Net;

Client::Client()
{
	SetComponentsFactory(CreateComponentsFactory<CClientComponentsFactory>());
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

	switch (static_cast<Net::PacketSCHeader>(header))
	{
		case PacketSCHeader::HEADER_SC_HANDSHAKE:
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

	TPacketSCHandshake handshakePacket;
	if (!CPacketIO::ReadPacketData(connectSocket.get(), handshakePacket))
		return false;

	std::cout << "Handshake Recv:" << handshakePacket.time << "\t" << handshakePacket.delta << std::endl;

	SendHandshake(handshakePacket.handshake, handshakePacket.time, handshakePacket.delta);

	return true;
}

void Client::SendHandshake(uint32_t handshake, uint32_t time, long delta)
{
	TPacketCSHandshake packet;
	packet.handshake = handshake;
	packet.time = time;
	packet.delta = delta;

	if (!CPacketIO::WritePacketData(GetSocket(), &packet, sizeof(packet)))
	{
		std::cerr << "Handshake not sended" << std::endl;
		return;
	}

	std::cout << "Handshake Send" << std::endl;
}
