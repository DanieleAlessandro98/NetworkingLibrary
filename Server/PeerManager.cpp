#include "StdAfx.h"
#include "PeerManager.h"
#include <Network/Definition.h>
#include <time.h>

CPeerManager::CPeerManager()
{
    m_iPeerConnected = 0;
    m_iHandleCount = 0;
    m_setHandshake.clear();
    m_mapPeer.clear();
}

bool CPeerManager::CanAcceptNewConnection()
{
    if (m_iPeerConnected >= SERVER_MAX_INCOMING_CONNECTIONS)
        return false;

    return true;
}

void CPeerManager::AcceptPeer(std::shared_ptr<Net::CSocket> socket, std::shared_ptr<Net::SocketWatcher> serverWatcher)
{
    auto newPeer = std::make_shared<CPeer>(serverWatcher);
    const auto handshake = __CreateHandshake();

    newPeer->Setup(socket, ++m_iHandleCount, handshake);

    m_setHandshake.emplace(handshake);
    m_mapPeer.emplace(newPeer->GetHandle(), newPeer);

    ++m_iPeerConnected;
}

uint32_t thecore_random()
{
    return rand() % (1024 * 1024);
}

uint32_t get_global_time()
{
    return static_cast<uint32_t>(time(0));
}

uint32_t GetCRC32(const char* buf, size_t len)
{
    uint32_t crc = 0;
    for (size_t i = 0; i < len; ++i) {
        crc += static_cast<uint32_t>(buf[i]);
    }
    return crc;
}

uint32_t CPeerManager::__CreateHandshake()
{
    char crc_buf[8];

    auto generate_crc = [&]() -> uint32_t {
        uint32_t val = thecore_random() % (1024 * 1024);
        *(uint32_t*)(crc_buf) = val;
        *((uint32_t*)crc_buf + 1) = get_global_time();
        return GetCRC32(crc_buf, 8);
    };

    uint32_t crc = generate_crc();
    while (crc == 0 || m_setHandshake.find(crc) != m_setHandshake.end())
        crc = generate_crc();

    return crc;
}
