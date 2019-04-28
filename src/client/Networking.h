#pragma once
#include "SDL_net.h"
#include <string>
class Networking
{
public:
	enum MessageType {
		POSITION_FOLLOWS,
		QUIT,
		REQ_DATA_SIZE_CHANGE,
		HIT
	};
	static bool startServer(Uint16 port);
	static bool stopServer();
	static bool connect(const std::string hostname, Uint16 port);
	static bool closeConnection();
	static void setDataSize(int newSize);
	static bool sendControlMsg(enum MessageType msg);
	static bool sendData(void* data);
	static bool recvControlMsg(enum MessageType* msg);
	static bool recvData(void* data);
	static bool acceptConnection();
private:
	Networking();
	~Networking();
	static int dataSize;
	static bool serverStarted;
	static bool isConnected;
	static IPaddress ip;
	static TCPsocket serverSock;
	static TCPsocket connectionSock;
};

