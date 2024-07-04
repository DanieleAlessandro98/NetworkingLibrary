#pragma once

namespace Net
{
	class CNetDevice
	{
		public:
			static void Destroy();
			static bool Create();

		private:
			static bool m_isWSA;
	};
}
