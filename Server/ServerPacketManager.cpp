#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketManager.h"

using namespace Net;

void CServerPacketManager::__LoadPacketHeaders()
{
	Set(PacketCGHeader::HEADER_CG_HANDSHAKE, CAbstractPacketManager::TPacketType(sizeof(TPacketCGHandshake), STATIC_SIZE_PACKET));
}
