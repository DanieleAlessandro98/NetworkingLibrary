#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ClientPacketManager.h"

using namespace Net;

CClientPacketManager::CClientPacketManager()
{
	__LoadPacketHeaders();
}

void CClientPacketManager::__LoadPacketHeaders()
{
	Set(PacketGCHeader::HEADER_GC_HANDSHAKE, CAbstractPacketManager::TPacketType(sizeof(TPacketGCHandshake), STATIC_SIZE_PACKET));
}
