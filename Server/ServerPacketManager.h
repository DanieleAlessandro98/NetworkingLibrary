#pragma once

#include <Network/AbstractServerPacketManager.h>

class CServerPacketManager : public Net::CAbstractServerPacketManager
{
	public:
		CServerPacketManager(Net::AbstractEntity* entity);
		~CServerPacketManager() = default;

	private:
		void __LoadPacketHeaders() override;
};
