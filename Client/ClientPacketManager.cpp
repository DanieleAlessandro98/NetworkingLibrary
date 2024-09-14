#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ClientPacketManager.h"

using namespace Net;

void CClientPacketManager::__LoadPacketHeaders()
{
	Set(PacketSCHeader::HEADER_SC_HANDSHAKE, CAbstractPacketManager::TPacketType(sizeof(TPacketSCHandshake), STATIC_SIZE_PACKET));
}
