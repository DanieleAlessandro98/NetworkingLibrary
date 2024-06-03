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
		void Initialize();

	protected:
		bool m_isWSA;
	};
}
