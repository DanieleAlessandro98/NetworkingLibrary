#include "StdAfx.h"
#include "DataStream.h"

namespace Net
{
	CDataStream::CDataStream(CSocket connectSocket)
	{
		this->connectSocket = connectSocket;

		sendBufInputPos = 0;
		sendBufOutputPos = 0;

		SetSendBufferSize(2048);
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
}
