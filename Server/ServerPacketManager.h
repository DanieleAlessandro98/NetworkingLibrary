#pragma once

#include <Network/AbstractServerPacketManager.h>

class CServerPacketManager : public Net::CAbstractServerPacketManager
{
	public:
		CServerPacketManager();
		~CServerPacketManager() = default;

	private:
		void __LoadPacketHeaders() override;
};
