#include<iostream>
#include<winSock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>
#include<vector>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

/*

//initialize winsock library

//create athe socket

//get ip and port

//bind the ip/port with the socket

//listen on  the socket

//accept

//recv and send

//cleanup the winsock

*/


bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data)==0;
}

void InteractWithClient(SOCKET clientSocket,vector<SOCKET>& clients) {
	//send/recv client
	cout << "client  conncted " << endl;
	char buffer[4096];

	while (1) {

		int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (bytesrecvd <= 0) {
			cout << "client disconnected" << endl;
			break;
		}

		string message(buffer, bytesrecvd);
		cout << "massage from  client : " << message << endl;

		for (auto client : clients) {//sabhi client par massage bhejna hai
			if (client != clientSocket) {//khud lo chhodkar sabko message bhejna hai
				send(client, message.c_str(), message.length(), 0);
			}
		}
	}

	auto it=find(clients.begin(), clients.end(), clientSocket);//after dissconnect all chats remove all chats from the vector
	if (it != clients.end()) {
		clients.erase(it);
	}

	closesocket(clientSocket);

}

int main() {

	//initialize winsock library
	if (!Initialize()) {
		cout << "winsock initialization failed " << endl;
		return 1;
	}



	cout << "server program" << endl;


	//CREATING SERVER SOCKET
	SOCKET listenSocket = socket(AF_INET,SOCK_STREAM,0);

	if (listenSocket == INVALID_SOCKET) {
		cout << "socket creation failed " << endl;
		return 1;
	}

	//create address structure
	int port = 12345;
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);

	//convert ip address(0.0.0.0) put it inside the sin_family in binary format
	if (InetPton(AF_INET, _T("0.0.0.0"),&serveraddr.sin_addr) != 1) {
		cout << "setting address structure failed " << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//bind
	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
		cout << "bind failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//listen
	if(listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "listen failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	cout << "server has started listening on port :" << port << endl;

	vector<SOCKET>clients;

	//accept-------------------------client request
	while (1) {


		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET) {
			cout << "invalid client socket " << endl;
		}

		clients.push_back(clientSocket);

		thread t1(InteractWithClient, clientSocket, std::ref(clients));
		t1.detach();
	}
	

	closesocket(listenSocket);






	WSACleanup();
	return 0;
}
