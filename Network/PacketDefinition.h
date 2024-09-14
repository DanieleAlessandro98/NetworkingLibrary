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

    enum class PacketCSHeader : TPacketHeader
    {
        HEADER_CS_HANDSHAKE = 1,
    };

    enum class PacketSCHeader : TPacketHeader
    {
        HEADER_SC_HANDSHAKE = 1,
    };

    struct TPacketSCHandshake
    {
        PacketSCHeader header = PacketSCHeader::HEADER_SC_HANDSHAKE;
        uint32_t	handshake;
        uint32_t	time;
        long	delta;
    };

    struct TPacketCSHandshake
    {
        PacketCSHeader header = PacketCSHeader::HEADER_CS_HANDSHAKE;
        uint32_t	handshake;
        uint32_t	time;
        long	delta;
    };
}
