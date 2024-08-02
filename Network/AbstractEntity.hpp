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

			void SetComponentsFactory(std::shared_ptr<AbstractComponentsFactory> factory)
			{
				m_componentsFactory = factory;
			}

		protected:
			std::shared_ptr<AbstractComponentsFactory> m_componentsFactory;
	};

	inline AbstractEntity::~AbstractEntity() {}
}
