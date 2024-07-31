#pragma once

namespace Net
{
    class CAbstractPacketHandler
    {
        public:
            CAbstractPacketHandler() = default;
            virtual ~CAbstractPacketHandler() = 0;
    };

    inline CAbstractPacketHandler::~CAbstractPacketHandler() {}
}
