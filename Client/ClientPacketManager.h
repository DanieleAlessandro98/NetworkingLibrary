#pragma once

#include <Network/AbstractClientPacketManager.h>

class CClientPacketManager : public Net::CAbstractClientPacketManager
{
	public:
		CClientPacketManager();
		~CClientPacketManager() = default;

	private:
		void __LoadPacketHeaders() override;
};
