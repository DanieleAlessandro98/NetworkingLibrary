#pragma once

#include <Network/AbstractPacketManager.h>

class CServerPacketManager : public Net::CAbstractPacketManager
{
	public:
		CServerPacketManager() = default;
		~CServerPacketManager() = default;

	private:
		void __LoadPacketHeaders() override;
};
