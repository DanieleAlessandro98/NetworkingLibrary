#pragma once

#include <map>
#include <memory>
#include "PacketDefinition.h"
#include "Socket.h"
#include "AbstractEntity.h"
#include "AbstractPeer.h"

namespace Net
{
	class CAbstractPacketManager
	{
		public:
			typedef struct SPacketType
			{
				int iPacketSize;
				bool isDynamicSizePacket;

				SPacketType(int iSize = 0, bool bFlag = false)
				{
					iPacketSize = iSize;
					isDynamicSizePacket = bFlag;
				}
			} TPacketType;

		public:
			CAbstractPacketManager() = default;
			virtual ~CAbstractPacketManager() = default;

			void Set(PacketCGHeader header, const TPacketType& packetType);
			void Set(PacketGCHeader header, const TPacketType& packetType);
			bool Get(TPacketHeader header, TPacketType* pPacketType);

			bool CheckPacket(CSocket* socket, TPacketHeader* packetHeader);
			bool ProcessRecv(CAbstractPacketBaseHandler* packetHandler);
			bool OnProcessRecv(CAbstractPacketBaseHandler* packetHandler);
			void RecvErrorPacket(CSocket* socket, int header);

		protected:
			enum
			{
				STATIC_SIZE_PACKET = false,
				DYNAMIC_SIZE_PACKET = true,
			};

			std::map<TPacketHeader, TPacketType> m_packetHeaderMap;

		private:
			void Set(TPacketHeader header, const TPacketType& rPacketType);
	};
}
