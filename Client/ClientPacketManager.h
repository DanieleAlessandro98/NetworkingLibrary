#pragma once

#include <Network/AbstractPacketManager.h>

class CClientPacketManager : public Net::CAbstractPacketManager
{
	public:
		CClientPacketManager();
		~CClientPacketManager() = default;

	private:
		void __LoadPacketHeaders() override;
};
