#pragma once

#include <vector>
#include "Definition.h"

namespace Net
{
	class CDataStream
	{
		public:
			CDataStream();
			~CDataStream() = default;

			void SetSendBufferSize(int sendBufSize);
			void SetRecvBufferSize(int recvBufSize);

			// Send funcs
			bool Send(int len, const void* pSrcBuf);
			bool Send(int size, const char* pSrcBuf);
			bool ProcessSend(SOCKET socket);
			void PopSendBuffer();


			// Recv funcs
			bool ProcessRecv(SOCKET socket);
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
			std::vector<char> m_sendBuf;
			int sendBufInputPos;
			int sendBufOutputPos;

			std::vector<char> m_recvBuf;
			int recvBufInputPos;
			int recvBufOutputPos;
	};
}
