#pragma once

#include <vector>
#include "Socket.h"

namespace Net
{
	class CDataStream
	{
		public:
			CDataStream(CSocket connectSocket);
			~CDataStream() = default;

			void SetSendBufferSize(int sendBufSize);

			// Send funcs
			bool Send(int len, const void* pSrcBuf);
			bool Send(int size, const char* pSrcBuf);
			bool ProcessSend();
			void PopSendBuffer();

			int GetSendBufInputPos() const;
			int GetSendBufOutpusPos() const;

		protected:
			int GetSendBufferSize();

		private:
			CSocket connectSocket;

			std::vector<char> m_sendBuf;
			int sendBufInputPos;
			int sendBufOutputPos;

	};
}
