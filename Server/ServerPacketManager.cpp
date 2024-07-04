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
	Set(PacketCGHeader::HEADER_CG_ACTION1, CAbstractPacketManager::TPacketType(sizeof(TPacketCGAction1), STATIC_SIZE_PACKET));
	Set(PacketCGHeader::HEADER_CG_ACTION2, CAbstractPacketManager::TPacketType(sizeof(TPacketCGAction2), STATIC_SIZE_PACKET));
	Set(PacketCGHeader::HEADER_CG_ACTION_WITH_SUBH, CAbstractPacketManager::TPacketType(sizeof(TPacketCGAction3WithSubH), DYNAMIC_SIZE_PACKET));
}
