#include "StdAfx.h"
#include "AbstractPeerManager.h"
#include "Utils.hpp"

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

    void CAbstractPeerManager::DestroyClosed()
    {
        std::vector<uint32_t> toErase;
        for (const auto& peer : m_mapPeer)
        {
            auto d = peer.second;
            if (d->IsPhase(PHASE_CLOSE))
                toErase.push_back(peer.first);
        }

        for (const auto& id : toErase)
        {
            auto it = m_mapPeer.find(id);
            if (it != m_mapPeer.end())
                DestroyDesc(it->second.get());
        }
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
            uint32_t val = Utils::GenerateRandomNumber();
            *(uint32_t*)(crc_buf) = val;
            *((uint32_t*)crc_buf + 1) = Utils::GetGlobalTime();
            return Utils::GenerateCRC32(crc_buf, 8);
        };

        uint32_t crc = generate_crc();
        while (crc == 0 || m_setHandshake.find(crc) != m_setHandshake.end())
            crc = generate_crc();

        return crc;
    }
}
