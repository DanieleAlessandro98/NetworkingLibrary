#pragma once

#include <vector>
#include <string>

namespace Net
{
	class CPacket
	{
		public:
			CPacket();
			~CPacket() = default;

			void Clear();
			void Append(const void* data, uint32_t size);
			void ResizeBuffer(size_t size);

			CPacket& operator << (const uint32_t data);
			CPacket& operator >> (uint32_t& data);

			CPacket& operator << (const std::string& data);
			CPacket& operator >> (std::string& data);

			size_t GetSize() const { return buffer.size(); }
			const char* GetData() const { return buffer.data(); }
			void* BeginBuffer() { return &buffer[0]; }

		private:
			std::vector<char> buffer;
			uint32_t extractionOffset;
	};
}
