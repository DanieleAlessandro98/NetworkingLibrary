#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ClientPacketHeaderMap.h"

using namespace Net;

CClientPacketHeaderMap::CClientPacketHeaderMap()
{
	__LoadPacketHeaders();
}

void CClientPacketHeaderMap::__LoadPacketHeaders()
{
	enum
	{
		STATIC_SIZE_PACKET = false,
		DYNAMIC_SIZE_PACKET = true,
	};

    Set(PacketGCHeader::HEADER_GC_SERVER_RESPONSE, CAbstractPacketHeaderMap::TPacketType(sizeof(TPacketGCServerResponse), DYNAMIC_SIZE_PACKET));
}
