#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ClientPacketManager.h"

using namespace Net;

CClientPacketManager::CClientPacketManager(AbstractEntity* entity)
{
	this->entity = entity;
	__LoadPacketHeaders();
}

void CClientPacketManager::__LoadPacketHeaders()
{
    Set(PacketGCHeader::HEADER_GC_SERVER_RESPONSE, CAbstractPacketManager::TPacketType(sizeof(TPacketGCServerResponse), DYNAMIC_SIZE_PACKET));
}
