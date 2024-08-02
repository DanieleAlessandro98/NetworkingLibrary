#include "StdAfx.h"
#include "AbstractPacketManager.h"
#include <iostream>

namespace Net
{
	void CAbstractPacketManager::Set(PacketCGHeader header, const TPacketType& packetType)
	{
		Set(static_cast<TPacketHeader>(header), packetType);
	}

	void CAbstractPacketManager::Set(PacketGCHeader header, const TPacketType& packetType)
	{
		Set(static_cast<TPacketHeader>(header), packetType);
	}

	void CAbstractPacketManager::Set(TPacketHeader header, const TPacketType& rPacketType)
	{
		m_packetHeaderMap[header] = rPacketType;
	}

	bool CAbstractPacketManager::Get(TPacketHeader header, TPacketType* pPacketType)
	{
		auto f = m_packetHeaderMap.find(header);
		if (m_packetHeaderMap.end() == f)
			return false;

		*pPacketType = f->second;

		return true;
	}

    bool CAbstractPacketManager::CheckPacket(CSocket* socket, TPacketHeader* packetHeader)
    {
        if (!socket)
            return false;

        const auto dataStream = socket->GetDataStream();
        if (!dataStream)
            return false;

        TPacketHeader tempHeader = 0;

        if (!dataStream->Peek(sizeof(TPacketHeader), &tempHeader))
            return false;

        if (0 == tempHeader)
        {
            if (!dataStream->Recv(sizeof(TPacketHeader), &tempHeader))
                return false;

            while (dataStream->Peek(sizeof(TPacketHeader), &tempHeader))
            {
                if (0 == tempHeader)
                {
                    if (!dataStream->Recv(sizeof(TPacketHeader), &tempHeader))
                        return false;
                }
                else
                    break;
            }

            if (tempHeader == 0)
                return false;
        }

        CAbstractPacketManager::TPacketType packetType;
        if (!Get(tempHeader, &packetType))
        {
            std::cerr << "Unknown packet header:" << std::endl;
            dataStream->ClearRecvBuffer();
            return false;
        }

        if (packetType.isDynamicSizePacket)
        {
            TDynamicSizePacketHeader dynamicSizePacketHeader;

            if (!dataStream->Peek(sizeof(TDynamicSizePacketHeader), &dynamicSizePacketHeader))
                return false;

            if (!dataStream->Peek(dynamicSizePacketHeader.size))
            {
                std::cerr << "Not enough dynamic packet size. Header: " << dynamicSizePacketHeader.header << " Packet size: " << dynamicSizePacketHeader.size << std::endl;
                return false;
            }
        }
        else
        {
            if (!dataStream->Peek(packetType.iPacketSize))
                return false;
        }

        if (!tempHeader)
            return false;

        *packetHeader = tempHeader;
        return true;
    }

    bool CAbstractPacketManager::ProcessRecv(CAbstractPacketBaseHandler* packetHandler)
    {
        if (!packetHandler)
            return false;

        const auto socket = packetHandler->GetSocket();
        if (!socket)
            return false;

        const auto dataStream = socket->GetDataStream();
        if (!dataStream)
            return false;

        if (!dataStream->ProcessRecv(socket->GetSocket()))
            return false;

        return OnProcessRecv(packetHandler);
    }

    bool CAbstractPacketManager::OnProcessRecv(CAbstractPacketBaseHandler* packetHandler)
    {
        if (!packetHandler)
            return false;

        const auto socket = packetHandler->GetSocket();
        if (!socket)
            return false;

        if (!socket)
            return false;

        TPacketHeader header = 0;

        bool ret = true;
        while (ret)
        {
            if (!CheckPacket(socket, &header))
                break;

            ret = packetHandler->AnalyzePacket(header);
        }

        if (!ret)
            RecvErrorPacket(socket, header);

        return ret;
    }

    void CAbstractPacketManager::RecvErrorPacket(CSocket* socket, int header)
    {
        if (!socket)
            return;

        const auto dataStream = socket->GetDataStream();
        if (!dataStream)
            return;

        std::cerr << "Not handled this header: " << static_cast<int>(header) << std::endl;
        dataStream->ClearRecvBuffer();
    }
}
