#pragma once

#include "AbstractPeer.h"
#include <unordered_map>
#include <unordered_set>
#include "AbstractComponentsFactory.h"

namespace Net
{
    class AbstractServerComponentsFactory;

    class CAbstractPeerManager
    {
        public:
            CAbstractPeerManager();
            ~CAbstractPeerManager();

            virtual bool CanAcceptNewConnection() = 0;
            virtual void OnPeerAccepted(CAbstractPeer* newPeer) = 0;

            void AcceptPeer(AbstractServerComponentsFactory* factory, std::shared_ptr<Net::CSocket> socket, std::shared_ptr<Net::SocketWatcher> serverWatcher);
            void DisconnectAll();
            void DestroyClosed();
            void DestroyDesc(CAbstractPeer* d, bool skipMapErase = false);

        protected:
            virtual uint32_t CreateHandshake();

            std::unordered_map<uint32_t, std::shared_ptr<CAbstractPeer>> m_mapPeer;
            std::unordered_set<uint32_t> m_setHandshake;

            int m_iPeerConnected;
            int m_iHandleCount;
            bool m_bDestroyed;

        public:
            template<typename T>
            static T* ValidPeer(CAbstractPeer* abstractPeer)
            {
                if (!abstractPeer)
                    return nullptr;

                auto* peer = static_cast<T*>(abstractPeer);
                if (!peer)
                    return nullptr;

                return peer;
            }
    };
}
