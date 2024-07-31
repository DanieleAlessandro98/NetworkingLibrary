#pragma once

#define SOCKET int
#define SERVER_MAX_INCOMING_CONNECTIONS 3

enum EPhase
{
	PHASE_CLOSE,
	PHASE_HANDSHAKE,
};
