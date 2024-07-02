#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketHeaderMap.h"

using namespace Net;

CServerPacketHeaderMap::CServerPacketHeaderMap()
{
    __LoadPacketHeaders();
}

void CServerPacketHeaderMap::__LoadPacketHeaders()
{
	enum
	{
		STATIC_SIZE_PACKET = false,
		DYNAMIC_SIZE_PACKET = true,
	};

    Set(PacketCGHeader::HEADER_CG_ACTION1, CAbstractPacketHeaderMap::TPacketType(sizeof(TPacketCGAction1), STATIC_SIZE_PACKET));
    Set(PacketCGHeader::HEADER_CG_ACTION2, CAbstractPacketHeaderMap::TPacketType(sizeof(TPacketCGAction2), STATIC_SIZE_PACKET));
    Set(PacketCGHeader::HEADER_CG_ACTION_WITH_SUBH, CAbstractPacketHeaderMap::TPacketType(sizeof(TPacketCGAction3WithSubH), DYNAMIC_SIZE_PACKET));
}
