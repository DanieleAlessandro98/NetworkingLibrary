#pragma once

#include <Network/AbstractClientPacketManager.h>

class CClientPacketManager : public Net::CAbstractClientPacketManager
{
	public:
		CClientPacketManager(Net::AbstractEntity* entity);
		~CClientPacketManager() = default;

	private:
		void __LoadPacketHeaders() override;
};
