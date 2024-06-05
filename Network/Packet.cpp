#include "StdAfx.h"
#include "Packet.h"
#include "PacketException.h"

namespace Net
{
	CPacket::CPacket()
	{
		Clear();
	}

	void CPacket::Clear()
	{
		buffer.clear();
		extractionOffset = 0;
	}

	void CPacket::Append(const void* data, uint32_t size)
	{
		buffer.insert(buffer.end(), (unsigned char*)data, (unsigned char*)data + size);
	}

	void CPacket::ResizeBuffer(size_t size)
	{
		buffer.resize(size);
	}

	CPacket& CPacket::operator<<(uint32_t data)
	{
		data = htonl(data);
		Append(&data, sizeof(uint32_t));
		return *this;
	}

	CPacket& CPacket::operator>>(uint32_t& data)
	{
		if ((extractionOffset + sizeof(uint32_t)) > buffer.size())
			throw PacketException("CPacket::operator uint32_t - extractionOffset exceeded buffer size.");

		//data = &buffer[extractionOffset]
		data = *reinterpret_cast<const uint32_t*>(&buffer[extractionOffset]);
		extractionOffset += sizeof(uint32_t);
		data = ntohl(data);

		return *this;
	}

	CPacket& CPacket::operator<<(const std::string& data)
	{
		*this << (uint32_t)data.size();
		Append(data.c_str(), data.size());

		return *this;
	}

	CPacket& CPacket::operator>>(std::string& data)
	{
		uint32_t stringSize = 0;
		*this >> stringSize;

		if ((extractionOffset + stringSize) > buffer.size())
			throw PacketException("CPacket::operator string - extractionOffset exceeded buffer size.");

		data.resize(stringSize);
		data.assign((char*)&buffer[extractionOffset], stringSize);
		extractionOffset += stringSize;

		return *this;
	}
}
