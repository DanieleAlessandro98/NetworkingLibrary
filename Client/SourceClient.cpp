#include "Network/NetDevice.h"
#include <iostream>

using namespace Net;

int main()
{
	std::unique_ptr<CNetDevice> netDevice = std::make_unique<CNetDevice>();

	bool resultCreate = netDevice->Create();
	if (!resultCreate)
		std::cerr << "Failed to create winsock";

	std::cout << "winsock created";

	netDevice->Destroy();

	return 0;
}
