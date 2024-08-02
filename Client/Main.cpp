#include "StdAfx.h"
#include <iostream>
#include <Network/NetDevice.h>
#include "Client.h"
#include <conio.h>

int main()
{
    if (!Net::CNetDevice::Create())
    {
        std::cerr << "Failed to create winsock" << std::endl;
        system("pause");
        return 0;
    }

	Client client;
	if (!client.Initialize("localhost", 8080))
	{
		std::cerr << "Failed to initialize client" << std::endl;
		system("pause");
		return 0;
	}

	while (true)
	{
		client.Process();

		if (client.IsConnected() && _kbhit())
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
		}
	}

    Net::CNetDevice::Destroy();

    system("pause");
    return 0;
}
