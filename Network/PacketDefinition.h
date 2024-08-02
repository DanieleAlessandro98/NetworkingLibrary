#pragma once

#include <cstdint>
#include <string>

namespace Net
{
    typedef uint8_t TPacketHeader;

    typedef struct packet_header_dynamic_size
    {
        uint8_t		header;
        uint16_t	size;
    } TDynamicSizePacketHeader;

    enum class PacketCGHeader : TPacketHeader
    {
        HEADER_CG_HANDSHAKE = 1,
    };

    enum class PacketGCHeader : TPacketHeader
    {
        HEADER_GC_HANDSHAKE = 1,
    };

    struct TPacketGCHandshake
    {
        PacketGCHeader header = PacketGCHeader::HEADER_GC_HANDSHAKE;
        uint32_t	handshake;
        uint32_t	time;
        long	delta;
    };

    struct TPacketCGHandshake
    {
        PacketCGHeader header = PacketCGHeader::HEADER_CG_HANDSHAKE;
        uint32_t	handshake;
        uint32_t	time;
        long	delta;
    };
}
