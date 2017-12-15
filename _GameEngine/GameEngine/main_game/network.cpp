#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>
#pragma warning( disable : 4996 C4996 )  
#define _WINSOCK_DEPRECATED_NO_WARNINGS


//number of clients needed to start networkings
// Number of Players = num+server = num+1
const int num = 2;
SOCKET Connections[num];
SOCKET Client_Connection;//This client's connection to the server

int ConnectionCounter = 0;

struct data {
	char buffer[256];
	int dx;
};

void processData(data dd) {
	//code to handle incoming data
	std::cout << dd.buffer << std::endl;
	//std::cout << " => " << dd.dx << std::endl;
}

void sendDataAsServer(data dout) {
	for (int i = 0; i < num; i++) //For each client connection
	{
		send(Connections[i], (char*)&dout, sizeof(data), NULL);//send the chat message to this client
	}
}

void ClientHandlerThread(int index) //index = the index in the SOCKET Connections array
{
	data ds;
	while (true)
	{
		recv(Connections[index], (char*)&ds, sizeof(data), NULL); //get message from client
		processData(ds);

		for (int i = 0; i < num; i++) //For each client connection
		{
			if (i == index) //Don't send the chat message to the same user who sent it
				continue; //Skip user
			send(Connections[i], (char*)&ds, sizeof(data), NULL);//send the chat message to this client
		}
	}
}

void init_server() {
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		//return 0;
	}

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //length of the address (required for accept call)
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //Bind the address to the socket
	listen(sListen, SOMAXCONN); //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections

	SOCKET newConnection; //Socket to hold the client's connection
	int ConnectionCounter = 0; //# of client connections
	for (int i = 0; i < num; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
		if (newConnection == 0) //If accepting the client connection failed
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else //If client connection properly accepted
		{
			std::cout << "Client Connected!" << std::endl;
			Connections[i] = newConnection; //Set socket in array to be the newest connection before creating the thread to handle this client's socket.
			ConnectionCounter += 1; //Incremenent total # of clients that have connected
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		}
	}
	std::cout << "Connection Bandwidth limit Reached " << std::endl;
}





void sendDataAsClient(data dout) {
	//sending data to all other clients
	//call this function to send data 
	send(Client_Connection, (char*)&dout, sizeof(data), NULL); //Send buffer
}

void ClientThread()
{
	data dc;
	while (true)
	{
		recv(Client_Connection, (char*)&dc, sizeof(data), NULL); //receive buffer
		processData(dc);
	}
}
void init_client() {
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		//return 0;
	}

	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Address = localhost (this pc)
	addr.sin_port = htons(1111); //Port = 1111
	addr.sin_family = AF_INET; //IPv4 Socket

	Client_Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Client_Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		//return 0; //Failed to Connect
	}

	std::cout << "Connected!" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	//Create the client thread that will receive any data that the server sends.

}


/*

int main()
{

	std::cout << "Master or Slave ,choose your way ?:V(m/s)" << std::endl;
	char ch;
	std::cin >> ch;

	if (ch == 'm') {
		init_server();
		data d;
		while (true)
		{
			std::cin.getline(d.buffer, sizeof(d.buffer)); //Get line if user presses enter and fill the buffer
			sendDataAsServer(d);
			Sleep(10);
		}
	}
	else {
		init_client();
		data d;
		while (true)
		{
			std::cin.getline(d.buffer, sizeof(d.buffer)); //Get line if user presses enter and fill the buffer
			d.dx = 2323;
			sendDataAsClient(d);
			Sleep(10);
		}
	}
	return 0;
}
*/