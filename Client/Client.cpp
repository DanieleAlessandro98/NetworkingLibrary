#include "StdAfx.h"
#include "Client.h"
#include <iostream>
#include <random>

using namespace Net;

Client::Client()
{
	m_packetManager = std::make_shared<CClientPacketManager>(this);
}

void Client::OnSocketCreated()
{
	std::cout << "socket created" << std::endl;
}

void Client::OnConnect()
{
	std::cout << "socket connected to the server" << std::endl;
}

void Client::OnConnectFail()
{
	std::cout << "OnConnectFailed" << std::endl;
}

void Client::OnDisconnect()
{
	std::cout << "OnRemoteDisconnect" << std::endl;
}

bool Client::Analyze(TPacketHeader header, std::shared_ptr<CSocket> socket)
{
	bool ret = true;

	switch (static_cast<Net::PacketGCHeader>(header))
	{
		case PacketGCHeader::HEADER_GC_SERVER_RESPONSE:
			ret = TestRecv();
			TestSendAction3();
			break;

		default:
			std::cerr << "Unknown packet header: " << static_cast<int>(header) << std::endl;
			ret = false;
			break;
	}

	return ret;
}

void Client::TestSend()
{
	if (!connectSocket)
		return;

	const auto dataStream = connectSocket->GetDataStream();
	if (!dataStream)
		return;

	TPacketCGAction1 action1;
	action1.numIntero = 5;
	if (!dataStream->Send(sizeof(action1), &action1))
	{
		std::cerr << "action1 not sended" << std::endl;
		return;
	}

	std::cout << "action1 sended" << std::endl;
}

bool Client::TestRecv()
{
	if (!connectSocket)
		return false;

	const auto dataStream = connectSocket->GetDataStream();
	if (!dataStream)
		return false;

	TPacketGCServerResponse responsePacket;
	if (!dataStream->Recv(sizeof(responsePacket), &responsePacket))
		return false;

	TPacketGCServerResponseDynamic responseSubPacket;
	if (!dataStream->Recv(sizeof(responseSubPacket), &responseSubPacket))
		return false;

	std::cout << "response receved. message: " << responseSubPacket.response << "with num recv with dynamicpacket = " << responseSubPacket.num << std::endl;

	return true;
}

int Client::__GetRandNumber()
{
	std::random_device rd;  // Dispositivo di entropia per il seme iniziale
	std::mt19937 gen(rd()); // Generatore di numeri pseudo-casuali Mersenne Twister

	// Definisce la distribuzione per generare numeri interi tra 1 e 3 inclusi
	std::uniform_int_distribution<> dis(1, 3);

	// Genera un numero casuale
	return dis(gen);
}

void Client::TestSendAction3()
{
	if (!connectSocket)
		return;

	const auto dataStream = connectSocket->GetDataStream();
	if (!dataStream)
		return;

	TPacketCGAction3WithSubH action3;
	action3.size = sizeof(action3);

	int random_number = __GetRandNumber();
	switch (random_number)
	{
		case 1:
		{
			action3.subheader = ACTION3_SUBHEADER_CG_1;

			TPacketCGAction3Sub1 sub;
			action3.size += sizeof(sub);
			sub.flag = true;

			if (!dataStream->Send(sizeof(action3), &action3))
			{
				std::cerr << "action3 not sended" << std::endl;
				return;
			}

			if (!dataStream->Send(sizeof(sub), &sub))
			{
				std::cerr << "action3-sub not sended" << std::endl;
				return;
			}

			std::cout << "action3 sended with sub = " << 1 << std::endl;
		}
		break;

		case 2:
		{
			action3.subheader = ACTION3_SUBHEADER_CG_2;

			TPacketCGAction3Sub2 sub;
			action3.size += sizeof(sub);
			sub.flag1 = true;
			sub.flag2 = true;

			if (!dataStream->Send(sizeof(action3), &action3))
			{
				std::cerr << "action3 not sended" << std::endl;
				return;
			}

			if (!dataStream->Send(sizeof(sub), &sub))
			{
				std::cerr << "action3-sub not sended" << std::endl;
				return;
			}

			std::cout << "action3 sended with sub = " << 2 << std::endl;
		}
		break;

		case 3:
		{
			action3.subheader = ACTION3_SUBHEADER_CG_3;

			TPacketCGAction3Sub3 sub;
			action3.size += sizeof(sub);
			sub.flag1 = true;
			sub.flag2 = true;
			sub.flag3 = true;

			if (!dataStream->Send(sizeof(action3), &action3))
			{
				std::cerr << "action3 not sended" << std::endl;
				return;
			}

			if (!dataStream->Send(sizeof(sub), &sub))
			{
				std::cerr << "action3-sub not sended" << std::endl;
				return;
			}

			std::cout << "action3 sended with sub = " << 3 << std::endl;
		}
		break;
	}
}
