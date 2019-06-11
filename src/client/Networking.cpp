#include "Networking.h"
#include <iostream>
#include "SDL_net.h"
int Networking::dataSize = 0;
bool Networking::serverStarted = false;
bool Networking::isConnected = false;
IPaddress Networking::ip = IPaddress();
TCPsocket Networking::serverSock = TCPsocket();
TCPsocket Networking::connectionSock = TCPsocket();

Networking::Networking()
{
}


Networking::~Networking()
{
}

bool Networking::startServer(Uint16 port)
{
	if (serverStarted)
		stopServer();
	SDLNet_ResolveHost(&ip, NULL, port);
	Networking::serverSock = SDLNet_TCP_Open(&ip);
	if (!serverSock) {
		std::cout << "Nie mozna utworzyc socketu: " << SDLNet_GetError() << std::endl;
	}
	serverStarted = true;
	return true;
}

bool Networking::stopServer()
{
	if (serverStarted) {
		SDLNet_TCP_Close(serverSock);
	}
	serverStarted = false;
	return true;
}

bool Networking::connect(const std::string hostname, Uint16 port)
{
	if (serverStarted)
		return false;
	SDLNet_ResolveHost(&ip, hostname.c_str(), port);
	connectionSock = SDLNet_TCP_Open(&ip);
	if (!connectionSock) {
		//std::cout << "Nie mozna utworzyc socketu polaczenia: " << SDLNet_GetError() << std::endl;
		return false;
	}
	isConnected = true;
	return true;
}

bool Networking::closeConnection()
{
	if (isConnected)
		SDLNet_TCP_Close(connectionSock);
	return true;
}

void Networking::setDataSize(int newSize)
{
	Networking::dataSize = newSize;
}

bool Networking::sendControlMsg(enum MessageType msg)
{
	if (isConnected) {
		int result = SDLNet_TCP_Send(connectionSock, &msg, sizeof(MessageType));
		if (result == sizeof(MessageType))
			return true;
	}
	return false;
}

bool Networking::sendData(void* data)
{
	if (isConnected) {
		int result = SDLNet_TCP_Send(connectionSock, data, dataSize);
		if (result == dataSize)
			return true;
	}
	return false;
}

bool Networking::recvControlMsg(enum MessageType* msg)
{
	if (!isConnected)
		return false;
	int result = SDLNet_TCP_Recv(connectionSock, msg, sizeof(MessageType));
	if (result == sizeof(MessageType)) {
		return true;
	}
	return false;
}

bool Networking::recvData(void* data)
{
	if (isConnected) {
		int result = SDLNet_TCP_Recv(connectionSock, data, dataSize);
		if (result == dataSize)
			return true;
	}
	return false;
}

bool Networking::acceptConnection()
{

	connectionSock = SDLNet_TCP_Accept(serverSock);
	if (connectionSock) {
		isConnected = true;
	}
	return connectionSock;
}
