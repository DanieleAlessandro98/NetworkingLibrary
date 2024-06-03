#include "Network/NetDevice.h"
#include <Network/Socket.h>
#include <iostream>

using namespace Net;

int main()
{
	std::unique_ptr<CNetDevice> netDevice = std::make_unique<CNetDevice>();

	if (netDevice->Create())
	{
		std::cout << "winsock created" << std::endl;

		CSocket listenSocket;
		if (listenSocket.Create())
		{
			std::cout << "socket created" << std::endl;
			if (listenSocket.Close())
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
