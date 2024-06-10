#include "StdAfx.h"
#include <iostream>
#include <Network/NetDevice.h>
#include "Server.h"

int main()
{
	Server server;

	Net::CNetDevice netDevice;
	if (!netDevice.Create())
	{
		std::cerr << "Failed to create winsock" << std::endl;
		system("pause");
		return 0;
	}

	if (server.Initialize("localhost", 8080))
	{
		while (true)
			server.Process();
	}

	netDevice.Destroy();

	system("pause");
	return 0;
}
