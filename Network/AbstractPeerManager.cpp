#include "StdAfx.h"
#include "AbstractPeerManager.h"
#include <time.h>

namespace Net
{
    CAbstractPeerManager::CAbstractPeerManager()
    {
        m_iPeerConnected = 0;
        m_iHandleCount = 0;
        m_bDestroyed = false;

        m_setHandshake.clear();
        m_mapPeer.clear();
    }

    CAbstractPeerManager::~CAbstractPeerManager()
    {
        if (m_bDestroyed)
            return;

        m_bDestroyed = true;

        DisconnectAll();
    }

    void CAbstractPeerManager::DisconnectAll()
    {
        for (const auto& peer : m_mapPeer)
        {
            auto d = peer.second;
            DestroyDesc(d.get(), true);
        }

        m_mapPeer.clear();
        m_setHandshake.clear();
    }

    void CAbstractPeerManager::DestroyDesc(CAbstractPeer* d, bool skipMapErase)
    {
        if (!d)
            return;

        if (!skipMapErase)
        {
            if (d->GetHandshake())
                m_setHandshake.erase(d->GetHandshake());

            if (d->GetHandle())
                m_mapPeer.erase(d->GetHandle());
        }

        --m_iPeerConnected;
    }

    uint32_t CAbstractPeerManager::CreateHandshake()
    {
        char crc_buf[8];

        auto generate_crc = [&]() -> uint32_t
        {
            uint32_t val = GenerateRandomNumber();
            *(uint32_t*)(crc_buf) = val;
            *((uint32_t*)crc_buf + 1) = GetGlobalTime();
            return GenerateCRC32(crc_buf, 8);
        };

        uint32_t crc = generate_crc();
        while (crc == 0 || m_setHandshake.find(crc) != m_setHandshake.end())
            crc = generate_crc();

        return crc;
    }

    uint32_t CAbstractPeerManager::GenerateCRC32(const char* buf, size_t len)
    {
        uint32_t crc = 0;
        for (size_t i = 0; i < len; ++i) {
            crc += static_cast<uint32_t>(buf[i]);
        }
        return crc;
    }

    uint32_t CAbstractPeerManager::GenerateRandomNumber()
    {
        return rand() % (1024 * 1024);
    }

    uint32_t CAbstractPeerManager::GetGlobalTime()
    {
        return static_cast<uint32_t>(time(0));
    }
}
