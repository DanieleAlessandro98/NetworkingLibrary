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
			void SetRecvBufferSize(int recvBufSize);

			// Send funcs
			bool Send(int len, const void* pSrcBuf);
			bool Send(int size, const char* pSrcBuf);
			bool ProcessSend();
			void PopSendBuffer();


			// Recv funcs
			bool ProcessRecv();
			bool Peek(int len, void* pDestBuf);
			bool Peek(int size, char* pDestBuf);
			bool Recv(int len, void* pDestBuf);
			bool Recv(int size, char* pDestBuf);
			bool Peek(int size);



			int GetSendBufInputPos() const;
			int GetSendBufOutpusPos() const;

			void ClearRecvBuffer();

		protected:
			int GetSendBufferSize();
			int GetRecvBufferSize();

		private:
			CSocket connectSocket;

			std::vector<char> m_sendBuf;
			int sendBufInputPos;
			int sendBufOutputPos;

			std::vector<char> m_recvBuf;
			int recvBufInputPos;
			int recvBufOutputPos;
	};
}
