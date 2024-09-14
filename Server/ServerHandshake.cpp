#include "StdAfx.h"
#include "ServerHandshake.h"
#include <iostream>
#include "Peer.h"
#include "PeerManager.h"
#include <Network/PacketIO.hpp>
#include "ServerComponentsFactory.hpp"

using namespace Net;

bool ServerHandshake::Analyze(CAbstractPeer* peer, TPacketHeader header)
{
	bool ret = true;

	switch (static_cast<PacketCSHeader>(header))
	{
		case PacketCSHeader::HEADER_CS_HANDSHAKE:
			ret = RecvHandshake(peer);
			break;

		default:
			std::cerr << "Unknown packet header: " << static_cast<int>(header) << std::endl;
			ret = false;
			break;
	}

	return ret;
}

bool ServerHandshake::RecvHandshake(CAbstractPeer* abstractPeer)
{
	auto* peer = CPeerManager::ValidPeer<ServerPeerType>(abstractPeer);
	if (!peer)
		return false;

	TPacketCSHandshake handshakePacket;
	if (!CPacketIO::ReadPacketData(peer->GetSocket(), handshakePacket))
		return false;

	if (peer->GetHandshake() != handshakePacket.handshake)
	{
		std::cerr << "Invalid Handshake" << std::endl;
		peer->SetPhase(PHASE_CLOSE);
		return false;
	}

	if (peer->HandshakeProcess(handshakePacket.time, handshakePacket.delta))
	{
		// New phase. Comment now
		//peer->SetPhase(PHASE_AUTH);
		std::cout << "Successful handshake" << std::endl;
	}

	return true;
}
