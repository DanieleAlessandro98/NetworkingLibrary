#include "StdAfx.h"
#include "PeerManager.h"
#include <Network/Definition.h>
#include <time.h>

bool CPeerManager::CanAcceptNewConnection()
{
    if (m_iPeerConnected >= SERVER_MAX_INCOMING_CONNECTIONS)
        return false;

    return true;
}

void CPeerManager::AcceptPeer(std::shared_ptr<Net::CSocket> socket, std::shared_ptr<Net::SocketWatcher> serverWatcher)
{
    auto newPeer = std::make_shared<CPeer>(serverWatcher);
    const auto handshake = CreateHandshake();

    newPeer->Setup(socket, ++m_iHandleCount, handshake);

    m_setHandshake.emplace(handshake);
    m_mapPeer.emplace(newPeer->GetHandle(), newPeer);

    ++m_iPeerConnected;
}

std::shared_ptr<Net::CAbstractPeer> CPeerManager::GetFirstPeer()
{
    if (m_mapPeer.empty())
        return nullptr;

    return m_mapPeer.begin()->second;
}
