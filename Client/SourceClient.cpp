#include "Network/NetDevice.h"
#include <Network/Socket.h>
#include <iostream>

using namespace Net;

int main()
{
	std::unique_ptr<CNetDevice> netDevice = std::make_unique<CNetDevice>();

	bool resultCreate = netDevice->Create();
	if (resultCreate)
	{
		std::cout << "winsock created" << std::endl;

		CSocket connectSocket;
		if (connectSocket.Create())
		{
			std::cout << "socket created" << std::endl;
			if (connectSocket.Close())
			{
				std::cout << "socket closed" << std::endl;
			}
			else
			{
				std::cerr << "Failed to close socket" << std::endl;
			}
		}
		else
		{
			std::cerr << "Failed to create socket" << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create winsock" << std::endl;
	}

	netDevice->Destroy();

	return 0;
}
