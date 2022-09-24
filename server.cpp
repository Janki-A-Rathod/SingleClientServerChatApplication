#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#define port 6000
#define MAX 500
using namespace std;

void Get_Data_From_Client(int x)
{

	int rMsgSize;
	char buff[MAX];
	while (1)
	{
		if ((rMsgSize = recv(x, buff, MAX, 0)) > 0)
		{
			cout << "received client : " << buff << endl;
			if (buff[0] == 'b' && buff[1] == 'y' && buff[2] == 'e')
			{
				cout << "Server : Bye bro" << endl;
				cout << "\nConnection ended... take care bye bye...\n";
				send(x, buff, strlen(buff) + 1, 0);
				exit(0);
			}
		}
	}
}
void Send_Data_To_Client(int y)
{
	while (1)
	{
		cout << "Server : ";
		char input[MAX];
		string s;
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

	/*First Process is to create a ServerSocket*/
	/*Function take 3 argument int family, int type, int protocol*/
	/*Family Contains ipv4 ipv6 and varities of family . Here we are using AF_INET->IPv4 protocols*/
	/*type − It specifies the kind of socket you want like stream socket,Datagram socket etc.Here we are using SOCK_STREAM (StreamSocket)
	used for TCP for data Transmission*/
	/*protocol − The argument should be set to the specific protocol type like TCP transport protocol,UDp transport Protocol.Here we are using 0 to select the system's default for the given combination of family and type */
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	// creating a socket and assigning it to the socket handler
	if (serverSocket < 0)
	{
		// socket methode return -1 if the creation was not successful
		cout << "Server Socket creation has failed.";
		return 0;
	}

	// specifying address and type for the server to operate under
	/*It represents an address family. In most of the Internet-based applications, we use AF_INET.*/
	/*sin_port	Service Port	A 16-bit port number in Network Byte Order.*/
	/*sin_addr	IP Address	A 32-bit IP address in Network Byte Order.*/
	/*INADDR_ANY value for IP address means the server's IP address will be assigned automatically.*/
	struct sockaddr_in serverAddr, clientAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/*The bind function assigns a local protocol address to a socket.
	 With the Internet protocols, the protocol address is the combination of either a 32-bit IPv4 address or a 128-bit IPv6 address,
	along with a 16-bit TCP or UDP port number. This function is called by TCP server only.*/
	/*(struct sockaddr*) & serverAddr->It is a pointer to struct sockaddr that contains the local IP address and port.*/
	int bindStatus = bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (bindStatus < 0)
	{
		cout << "Socket binding has failed" << endl;
		return 0;
	}
	/* Now start listening for the clients, here process will go in sleep mode and will wait for the incoming connection*/
	/*The listen function is called only by a TCP server and it performs two actions −
	1.The listen function converts an unconnected socket into a passive socket,
	indicating that the kernel should accept incoming connection requests directed to this socket.
	2.The second argument to this function specifies the maximum number of connections the kernel should queue for this socket.*/
	/*serverSocket-> It is a socket descriptor returned by the socket function.*/
	/*5->It is the number of allowed connections.*/
	int listenStatus = listen(serverSocket, 5);
	if (listenStatus < 0)
	{ // when queue is full listen fails
		cout << "Listner has failed" << endl;
		return 0;
	}
	cout << "\t\t...Waiting for connections... \n\n";
	socklen_t len = sizeof(clientAddr);
	int connection;
	if ((connection = accept(serverSocket, (struct sockaddr *)&clientAddr, &len)) < 0)
	{
		cout << "Server didn't accept the request." << endl;
		return 0;
	}
	else
	{
		cout << "Server accepted the request. \n";
	}
	while (true)
	{
		// infinite loop for chatting
		thread th1(Get_Data_From_Client, connection);
		thread th2(Send_Data_To_Client, connection);
		th1.join();
		th2.join();
	}
	close(serverSocket);
	return 0;
}
