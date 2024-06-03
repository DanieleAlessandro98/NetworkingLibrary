#include "StdAfx.h"
#include "NetDevice.h"

namespace Net
{
	CNetDevice::CNetDevice()
	{
		m_isWSA = false;
	}

	CNetDevice::~CNetDevice()
	{
		Destroy();
	}

	void CNetDevice::Destroy()
	{
		if (m_isWSA)
		{
			WSACleanup();
			m_isWSA = false;
		}
	}

	bool CNetDevice::Create()
	{
		Destroy();

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			return false;

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			WSACleanup();
			return false;
		}

		m_isWSA = true;

		return true;
	}
}
