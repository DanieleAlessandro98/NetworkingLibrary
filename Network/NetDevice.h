#pragma once

namespace Net
{
	class CNetDevice
	{
	public:
		CNetDevice();
		virtual ~CNetDevice();

		void Destroy();
		bool Create();

	protected:
		bool m_isWSA;
	};
}
