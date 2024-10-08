#pragma once

#include <map>
#include <memory>
#include "PacketDefinition.h"
#include "Socket.h"
#include "AbstractEntity.hpp"
#include "AbstractPeer.h"

namespace Net
{
	class CAbstractPacketManager
	{
		#define DEFINE_PACKET(header, structType, sizeType) Set(header, sizeof(structType), sizeType)

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
			void Initialize();

			virtual void __LoadPacketHeaders() = 0;

			void Set(PacketCSHeader header, size_t packetSize, bool isDynamic);
			void Set(PacketSCHeader header, size_t packetSize, bool isDynamic);
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
