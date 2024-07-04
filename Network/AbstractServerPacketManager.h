#pragma once

#include "AbstractPacketManager.h"

namespace Net
{
	class CAbstractServerPacketManager : public CAbstractPacketManager
	{
		public:
			CAbstractServerPacketManager() = default;
			virtual ~CAbstractServerPacketManager() = 0;

		private:
			virtual void __LoadPacketHeaders() = 0;
	};

	inline CAbstractServerPacketManager::~CAbstractServerPacketManager() {}
}
