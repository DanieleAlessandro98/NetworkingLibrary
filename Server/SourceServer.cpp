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

			CSocket listenSocket;
			if (listenSocket.Create())
			{
				std::cout << "socket created" << std::endl;

				CNetAddress netAddress;
				if (netAddress.Set("localhost", 8080))
				{
					std::cout << "IP: " << netAddress.GetIP() << std::endl;

					if (listenSocket.Listen(netAddress))
					{
						std::cout << "socket listening" << std::endl;

						CSocket newClientSocket;
						if (listenSocket.Accept(newClientSocket))
						{
							std::cout << "new client accepted" << std::endl;

							for (int i = 0; i < 5; i++)
							{
								char buffer[256];
								if (newClientSocket.RecvAll(buffer, 256))
								{
									std::cout << buffer << std::endl;

									char buffer1[256];
									strcpy_s(buffer1, "Data received!\0");
									if (!newClientSocket.SendAll(buffer1, 256))
										std::cerr << "Failed to send" << std::endl;
								}
								else
									std::cerr << "Failed to recv" << std::endl;
							}

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
							std::cerr << "Failed to accept new client" << std::endl;
						}
					}
					else
					{
						std::cerr << "Failed to listen socket" << std::endl;
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
