#pragma once

#include "AbstractPacketManager.h"

namespace Net
{
	class CAbstractClientPacketManager : public CAbstractPacketManager
	{
		public:
			CAbstractClientPacketManager() = default;
			virtual ~CAbstractClientPacketManager() = 0;

		private:
			virtual void __LoadPacketHeaders() = 0;
	};

	inline CAbstractClientPacketManager::~CAbstractClientPacketManager() {}
}
