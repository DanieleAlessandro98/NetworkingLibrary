#include "StdAfx.h"
#include "ServerHandshake.h"
#include <iostream>
#include "Peer.h"
#include "PeerManager.h"
#include <Network/PacketIO.hpp>

using namespace Net;

bool ServerHandshake::Analyze(CAbstractPeer* peer, TPacketHeader header)
{
	bool ret = true;

	switch (static_cast<PacketCGHeader>(header))
	{
		case PacketCGHeader::HEADER_CG_HANDSHAKE:
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
	auto* peer = CPeerManager::ValidPeer<CPeer>(abstractPeer);
	if (!peer)
		return false;

	TPacketCGHandshake handshakePacket;
	if (!CPacketIO::ReadPacketData(peer->GetSocket(), handshakePacket))
		return false;

	if (peer->GetHandshake() != handshakePacket.handshake)
	{
		std::cerr << "Invalid Handshake" << std::endl;
		return false;
	}

	if (peer->IsPhase(PHASE_HANDSHAKE))
	{
		if (peer->HandshakeProcess(handshakePacket.time, handshakePacket.delta))
		{
			// New phase. Comment now
			//peer->SetPhase(PHASE_AUTH);
			std::cout << "Successful handshake" << std::endl;
		}
	}

	return true;
}
