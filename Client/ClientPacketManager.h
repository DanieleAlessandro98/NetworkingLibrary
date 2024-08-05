#pragma once

#include <Network/AbstractPacketManager.h>

class CClientPacketManager : public Net::CAbstractPacketManager
{
	public:
		CClientPacketManager() = default;
		~CClientPacketManager() = default;

	private:
		void __LoadPacketHeaders() override;
};
