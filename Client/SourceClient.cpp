#include "Network/NetDevice.h"
#include "Network/NetAddress.h"
#include "Network/Socket.h"
#include <iostream>

using namespace Net;

int main()
{
	std::unique_ptr<CNetDevice> netDevice = std::make_unique<CNetDevice>();
	if (netDevice->Create())
	{
		try
		{
			std::cout << "winsock created" << std::endl;

			CSocket connectSocket;
			if (connectSocket.Create())
			{
				std::cout << "socket created" << std::endl;

				std::unique_ptr<CNetAddress> netAddress = std::make_unique<CNetAddress>();
				if (netAddress->Set("localhost", 8080))
				{
					std::cout << "IP: " << netAddress->GetIP() << std::endl;

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
					std::cerr << "Failed to set address" << std::endl;
				}
			}
			else
			{
				std::cerr << "Failed to create socket" << std::endl;
			}
		}
		catch (NetException ex)
		{
			std::cerr << "Exception: " << ex.what() << std::endl;
			netDevice->Destroy();
		}
	}
	else
	{
		std::cerr << "Failed to create winsock" << std::endl;
	}

	netDevice->Destroy();

	return 0;
}
