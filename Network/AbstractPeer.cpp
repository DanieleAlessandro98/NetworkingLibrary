#include "StdAfx.h"
#include "AbstractPeer.h"

namespace Net
{
	void CAbstractPeer::AddSocketToWatcher(int fd)
	{
		m_serverWatcher->add_fd(fd, this, Net::EFdwatch::FDW_READ, false);
	}

	void CAbstractPeer::RemoveSocketToWatcher(int fd)
	{
		m_serverWatcher->remove_fd(fd);
	}
}
