#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketManager.h"

using namespace Net;

void CServerPacketManager::__LoadPacketHeaders()
{
	Set(PacketCSHeader::HEADER_CS_HANDSHAKE, CAbstractPacketManager::TPacketType(sizeof(TPacketCSHandshake), STATIC_SIZE_PACKET));
}
