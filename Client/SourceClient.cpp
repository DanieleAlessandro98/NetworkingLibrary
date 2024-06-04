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

				CNetAddress netAddress;
				if (netAddress.Set("localhost", 8080))
				{
					std::cout << "IP: " << netAddress.GetIP() << std::endl;

					if (connectSocket.Connect(netAddress))
					{
						std::cout << "socket connected to the server" << std::endl;

						for (int i = 0; i < 5; i++)
						{
							char buffer[256];
							strcpy_s(buffer, "Hello world from client!\0");
							if (connectSocket.SendAll(buffer, 256))
							{
								std::cout << "Sending data to the server..." << std::endl;
								char buffer1[256];
								if (connectSocket.RecvAll(buffer1, 256))
									std::cout << buffer1 << std::endl;
							}
							else
								std::cerr << "Failed to send" << std::endl;
						}

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
						std::cerr << "Failed to connect to the server" << std::endl;
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

	system("pause");
	return 0;
}
