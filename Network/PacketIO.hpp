#pragma once

#include "Socket.h"

namespace Net
{
	class CPacketIO
	{
		public:
			static bool WritePacketData(CSocket* socket, const void* pData, int dataSize)
			{
				if (!socket)
					return false;

				const auto dataStream = socket->GetDataStream();
				if (!dataStream)
					return false;

				if (!dataStream->Send(dataSize, pData))
					return false;

				return true;
			}

			template <typename PacketStruct>
			static bool ReadPacketData(CSocket* socket, PacketStruct& outData)
			{
				if (!socket)
					return false;

				const auto dataStream = socket->GetDataStream();
				if (!dataStream)
					return false;

				if (!dataStream->Recv(sizeof(outData), &outData))
					return false;

				return true;
			}
	};
}
