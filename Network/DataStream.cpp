#include "StdAfx.h"
#include "DataStream.h"

namespace Net
{
	CDataStream::CDataStream(CSocket connectSocket)
	{
		this->connectSocket = connectSocket;

		sendBufInputPos = 0;
		sendBufOutputPos = 0;

		recvBufInputPos = 0;
		recvBufOutputPos = 0;

		SetSendBufferSize(2048);
		SetRecvBufferSize(128 * 1024);
	}

	void CDataStream::SetSendBufferSize(int sendBufSize)
	{
		if (!m_sendBuf.empty())
		{
			if (m_sendBuf.size() > sendBufSize)
				return;
		}

		m_sendBuf.clear();
		m_sendBuf.resize(sendBufSize);
	}

	void CDataStream::SetRecvBufferSize(int recvBufSize)
	{
		if (!m_recvBuf.empty())
		{
			if (m_recvBuf.size() > recvBufSize)
				return;
		}

		m_recvBuf.clear();
		m_recvBuf.resize(recvBufSize);
	}

	bool CDataStream::Send(int len, const void* pSrcBuf)
	{
		return Send(len, static_cast<const char*>(pSrcBuf));
	}

	bool CDataStream::Send(int size, const char* pSrcBuf)
	{
		const int sendBufRestSize = m_sendBuf.size() - sendBufInputPos;
		if ((size + 1) > sendBufRestSize)
			return false;

		memcpy(m_sendBuf.data() + sendBufInputPos, pSrcBuf, size);
		sendBufInputPos += size;

		return true;
	}

	bool CDataStream::ProcessSend()
	{
		int dataSize = GetSendBufferSize();
		if (dataSize <= 0)
			return true;

		int sendSize = send(connectSocket.GetSocket(), m_sendBuf.data() + sendBufOutputPos, dataSize, 0);
		if (sendSize < 0)
			return false;

		sendBufOutputPos += sendSize;

		PopSendBuffer();
		return true;
	}

	void CDataStream::PopSendBuffer()
	{
		if (sendBufOutputPos <= 0)
			return;

		const auto sendBufDataSize = GetSendBufferSize();

		if (sendBufDataSize > 0)
			memmove(m_sendBuf.data(), m_sendBuf.data() + sendBufOutputPos, sendBufDataSize);

		sendBufInputPos = sendBufDataSize;
		sendBufOutputPos = 0;
	}

	bool CDataStream::ProcessRecv()
	{
		int dataSize = m_recvBuf.size() - recvBufInputPos;
		if (dataSize <= 0)
			return true;

		int recvSize = recv(connectSocket.GetSocket(), m_recvBuf.data() + recvBufInputPos, dataSize, 0);
		if (recvSize <= 0)
		{
			if (recvSize == 0 || WSAGetLastError() != WSAEWOULDBLOCK)
				return false;
		}

		recvBufInputPos += recvSize;
	}

	bool CDataStream::Peek(int len, void* pDestBuf)
	{
		return Peek(len, (char*)pDestBuf);
	}

	bool CDataStream::Peek(int size, char* pDestBuf)
	{
		if (GetRecvBufferSize() < size)
			return false;

		memcpy(pDestBuf, m_recvBuf.data() + recvBufOutputPos, size);
		return true;
	}

	bool CDataStream::Recv(int len, void* pDestBuf)
	{
		return Recv(len, (char*)pDestBuf);
	}

	bool CDataStream::Recv(int size, char* pDestBuf)
	{
		if (!Peek(size, pDestBuf))
			return false;

		recvBufOutputPos += size;
		return true;
	}

	bool CDataStream::Peek(int size)
	{
		if (GetRecvBufferSize() < size)
			return false;

		return true;
	}


	int CDataStream::GetSendBufInputPos() const
	{
		return sendBufInputPos;
	}

	int CDataStream::GetSendBufOutpusPos() const
	{
		return sendBufOutputPos;
	}

	int CDataStream::GetSendBufferSize()
	{
		return sendBufInputPos - sendBufOutputPos;
	}

	int CDataStream::GetRecvBufferSize()
	{
		return recvBufInputPos - recvBufOutputPos;
	}

	void CDataStream::ClearRecvBuffer()
	{
		recvBufInputPos = recvBufOutputPos = 0;
	}
}
