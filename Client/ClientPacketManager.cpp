#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ClientPacketManager.h"

using namespace Net;

void CClientPacketManager::__LoadPacketHeaders()
{
	DEFINE_PACKET(PacketSCHeader::HEADER_SC_HANDSHAKE, TPacketSCHandshake, STATIC_SIZE_PACKET);
}
