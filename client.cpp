#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#define port 6000
#define MAX 500
using namespace std;
void Get_Data_From_Server(int x)
{
	char receiveMessage[MAX];
	while (1)
	{
		int rMsgSize = recv(x, receiveMessage, MAX, 0);
		if (rMsgSize < 0)
		{
			cout << "Packet recieve failed." << endl;
			// return 0;
		}
		else if (rMsgSize == 0)
		{
			cout << "Server is off." << endl;
			// return 0;
		}

		if (receiveMessage[0] == 'b' && receiveMessage[1] == 'y' && receiveMessage[2] == 'e')
		{
			cout << "\nConnection ended... take care bye bye... ";
			exit(0);
		}
		cout << "received server : " << receiveMessage << endl;
	}
}
void Send_Data_To_Server(int y)
{
	string s;
	char input[MAX];
	while (1)
	{
		cout << "Client : ";
		getline(cin, s);
		int n = s.size();
		for (int i = 0; i < n; i++)
		{
			input[i] = s[i];
		}
		input[n] = '\0';
		send(y, input, strlen(input) + 1, 0);
	}
}
int main()
{
	/*First Process is to create a clientSocket*/
	/*Function take 3 argument int family, int type, int protocol*/
	/*Family Contains ipv4 ipv6 and varities of family . Here we are using AF_INET->IPv4 protocols*/
	/*type − It specifies the kind of socket you want like stream socket,Datagram socket etc.Here we are using SOCK_STREAM (StreamSocket)
	used for TCP for data Transmission*/
	/*protocol − The argument should be set to the specific protocol type like TCP transport protocol,UDp transport Protocol.Here we are using 0 to select the system's default for the given combination of family and type */
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	// creating a socket and assigning it to the socket handler
	if (clientSocket < 0)
	{
		// socket methode return -1 if the creation was not successful
		cout << "Client Socket creation has failed.";
		return 0;
	}
	/*sin_family = specifies the address family, usually the constant AF_INET*/
	/*sin_addr = holds the IP address returned by inet_addr() to be used in the socket connection.*/
	/*sin_port = specifies the port number and must be used with htons() function that converts the host byte order to network byte order so that it can be transmitted and routed properly when opening the socket connection. The reason for this is that computers and network protocols order their bytes in a non-compatible fashion*/
	struct sockaddr_in serverAddr, clientAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(port);
	/*serv_addr − It is a pointer to struct sockaddr that contains destination IP address and port.*/
	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		cout << "Connection Error..." << endl;
		return 0;
	}
	else
	{
		cout << "\t\tConnection Established..." << endl;
	}
	while (true)
	{
		thread th1(Get_Data_From_Server, clientSocket);
		thread th2(Send_Data_To_Server, clientSocket);
		th1.join();
		th2.join();
	}
	close(clientSocket);
	return 0;
}
