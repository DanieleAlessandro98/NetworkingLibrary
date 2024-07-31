#include "StdAfx.h"
#include "AbstractPeer.h"

namespace Net
{
	void CAbstractPeer::AddSocketToWatcher(int fd, CAbstractPeer* client_data)
	{
		m_serverWatcher->add_fd(fd, this, Net::EFdwatch::FDW_READ, false);
	}
}
