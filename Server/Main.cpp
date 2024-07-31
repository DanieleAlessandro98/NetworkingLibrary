#include "StdAfx.h"
#include <iostream>
#include <Network/NetDevice.h>
#include "ServerMain.h"
#include <conio.h>

int main()
{
	ServerMain server;

	if (!Net::CNetDevice::Create())
	{
		std::cerr << "Failed to create winsock" << std::endl;
		system("pause");
		return 0;
	}

	if (server.Initialize("localhost", 8080))
	{
		while (true)
		{
			server.Process();

			if (_kbhit())
			{
				std::string input;

				char ch = _getch();
				while (ch != '\r')
				{
					input += ch;
					std::cout << ch;
					ch = _getch();
				}

				std::cout << std::endl;

				if (input == "dall")
					server.DisconnectAll();
				if (input == "dfirst")
					server.DisconnectFirstPeer();
				else if (input == "quit")
					break;
			}
		}
	}

	server.Shutdown();
	Net::CNetDevice::Destroy();

	system("pause");
	return 0;
}
