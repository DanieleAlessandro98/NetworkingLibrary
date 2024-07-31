#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketManager.h"

using namespace Net;

CServerPacketManager::CServerPacketManager(Net::AbstractEntity* entity)
{
	this->entity = entity;
	__LoadPacketHeaders();
}

void CServerPacketManager::__LoadPacketHeaders()
{
}
