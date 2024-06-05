#include "Network/NetDevice.h"
#include "Network/NetAddress.h"
#include "Network/Socket.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <Network/PacketDefinition.h>

using namespace Net;

std::string random_string(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

int main()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(3, 15);

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

						while (true)
						{
							TPacketAction1 action1;
							action1.numIntero = 5;
							if (!connectSocket.Send(action1))
								break;

							std::cout << "action1 sended" << std::endl;
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
