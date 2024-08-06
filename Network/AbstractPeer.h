#pragma once

#include "Socket.h"
#include "SocketWatcher.h"
#include "PacketDefinition.h"
#include "AbstractPacketHandler.h"
#include <unordered_map>

namespace Net
{
    class SocketWatcher;
    class CAbstractPacketBaseHandler;

    class CAbstractPeer : public CAbstractPacketBaseHandler
    {
        public:
            CAbstractPeer(std::shared_ptr<SocketWatcher> serverWatcher);
            ~CAbstractPeer();

            void RegisterPhaseHandler(int phase, CAbstractPacketServerHandler* handler);
            void SetPhase(int phase);

            virtual void OnSetupCompleted() = 0;

            void AddSocketToWatcher(int fd);
            void RemoveSocketToWatcher(int fd);

            void Setup(std::shared_ptr<CSocket> socket, int handleCount, uint32_t handshake);
            void Packet(const void* c_pvData, int iSize);

            bool AnalyzePacket(TPacketHeader header) override;

            CSocket* GetSocket() override { return m_socket.get(); }
            uint32_t GetHandle() const { return m_dwHandle; }
            uint32_t GetHandshake() const { return m_dwHandshake; }
            bool IsPhase(int phase) const { return m_iPhase == phase ? true : false; }

        protected:
            std::shared_ptr<SocketWatcher>	m_serverWatcher;
            bool			m_bDestroyed;
            int				m_iPhase;
            int								m_dwHandle;
            uint32_t						m_dwHandshake;
            std::shared_ptr<CSocket>	m_socket;

            CAbstractPacketServerHandler* m_packetHandler;
            std::unordered_map<int, CAbstractPacketServerHandler*> m_phaseHandlers;
    };
}
