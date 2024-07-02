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
        HEADER_CG_ACTION1 = 1,
        HEADER_CG_ACTION2 = 2,
        HEADER_CG_ACTION_WITH_SUBH = 3,
    };

    enum class PacketGCHeader : TPacketHeader
    {
        HEADER_GC_SERVER_RESPONSE = 10,
    };

    struct TPacketCGAction1
    {
        PacketCGHeader header = PacketCGHeader::HEADER_CG_ACTION1;
        uint32_t numIntero;
    };

    struct TPacketCGAction2
    {
        PacketCGHeader header = PacketCGHeader::HEADER_CG_ACTION2;
        uint32_t numIntero;
    };

    struct TPacketCGAction3WithSubH
    {
        PacketCGHeader header = PacketCGHeader::HEADER_CG_ACTION_WITH_SUBH;
        uint16_t size;
        TPacketHeader subheader;
    };

    enum EPacketCGAction3SubHeader : TPacketHeader
    {
        ACTION3_SUBHEADER_CG_1,
        ACTION3_SUBHEADER_CG_2,
        ACTION3_SUBHEADER_CG_3,
    };

    struct TPacketCGAction3Sub1
    {
        bool flag;
    };

    struct TPacketCGAction3Sub2
    {
        bool flag1;
        bool flag2;
    };

    struct TPacketCGAction3Sub3
    {
        bool flag1;
        bool flag2;
        bool flag3;
    };

    struct TPacketGCServerResponse
    {
        PacketGCHeader header = PacketGCHeader::HEADER_GC_SERVER_RESPONSE;
        uint16_t size;
    };

    struct TPacketGCServerResponseDynamic
    {
        char response[50];
        int num;
    };
}
