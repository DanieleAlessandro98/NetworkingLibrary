#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketManager.h"

using namespace Net;

void CServerPacketManager::__LoadPacketHeaders()
{
	DEFINE_PACKET(PacketCSHeader::HEADER_CS_HANDSHAKE, TPacketCSHandshake, STATIC_SIZE_PACKET);
}
