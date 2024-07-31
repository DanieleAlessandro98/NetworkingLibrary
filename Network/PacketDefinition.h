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

    };

    enum class PacketGCHeader : TPacketHeader
    {

    };
}
