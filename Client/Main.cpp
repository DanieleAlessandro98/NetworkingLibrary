#include "StdAfx.h"
#include <iostream>
#include <Network/NetDevice.h>
#include "Client.h"

int main()
{
	Client client;

	Net::CNetDevice netDevice;
	if (!netDevice.Create())
	{
		std::cerr << "Failed to create winsock" << std::endl;
		system("pause");
		return 0;
	}

    if (client.Initialize("localhost", 8080))
    {
        while (true)
        {
            client.Process();

            if (client.IsConnected())
            {
                std::string input;
                std::getline(std::cin, input);

                if (input == "ok")
                    client.TestSend();
            }
        }
    }

	netDevice.Destroy();

	system("pause");
	return 0;
}
