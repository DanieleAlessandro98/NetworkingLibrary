#pragma once

#include "AbstractPeer.h"
#include <unordered_map>
#include <unordered_set>

namespace Net
{
    class CAbstractPeerManager
    {
        public:
            CAbstractPeerManager();
            ~CAbstractPeerManager();

            virtual bool CanAcceptNewConnection() = 0;
            virtual void AcceptPeer(std::shared_ptr<CSocket> socket, std::shared_ptr<SocketWatcher> serverWatcher) = 0;

            void DisconnectAll();
            void DestroyDesc(CAbstractPeer* d, bool skipMapErase = false);

        protected:
            virtual uint32_t CreateHandshake();

            std::unordered_map<uint32_t, std::shared_ptr<CAbstractPeer>> m_mapPeer;
            std::unordered_set<uint32_t> m_setHandshake;

            int m_iPeerConnected;
            int m_iHandleCount;
            bool m_bDestroyed;

        private:
            uint32_t GenerateCRC32(const char* buf, size_t len);
            uint32_t GenerateRandomNumber();
            uint32_t GetGlobalTime();
    };
}
