#include "StdAfx.h"
#include "PeerManager.h"
#include <Network/Definition.h>
#include <time.h>
#include <iostream>

bool CPeerManager::CanAcceptNewConnection()
{
    if (m_iPeerConnected >= SERVER_MAX_INCOMING_CONNECTIONS)
        return false;

    return true;
}

void CPeerManager::OnPeerAccepted(Net::CAbstractPeer* newPeer)
{
    if (!newPeer)
        return;

    std::cout << "New peer accepted" << std::endl;
}

std::shared_ptr<Net::CAbstractPeer> CPeerManager::GetFirstPeer()
{
    if (m_mapPeer.empty())
        return nullptr;

    return m_mapPeer.begin()->second;
}
