#pragma once

#include <Network/AbstractPacketManager.h>

class CServerPacketManager : public Net::CAbstractPacketManager
{
	public:
		CServerPacketManager();
		~CServerPacketManager() = default;

	private:
		void __LoadPacketHeaders() override;
};
