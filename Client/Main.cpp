#include "StdAfx.h"
#include <iostream>
#include <Network/NetDevice.h>
#include "Client.h"
#include <conio.h>

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

                if (input == "ok")
                    client.TestSend();
            }
        }
    }

    netDevice.Destroy();

    system("pause");
    return 0;
}
