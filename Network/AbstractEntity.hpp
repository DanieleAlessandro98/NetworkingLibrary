#pragma once

#include "PacketDefinition.h"
#include "Socket.h"
#include "AbstractComponentsFactory.h"

namespace Net
{
	class AbstractEntity
	{
		public:
			AbstractEntity() = default;
			virtual ~AbstractEntity() = 0;

			virtual bool Initialize(const char* c_szAddr, int port) = 0;
			virtual void Process() = 0;
			virtual void Shutdown() = 0;
	};

	inline AbstractEntity::~AbstractEntity() {}
}
