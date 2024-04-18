//https://www.geeksforgeeks.org/socket-programming-in-cpp/

// C++ program to show the example of server application in
// socket programming 
#include <cstring> 
#include <iostream>
#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
#endif
#include <unistd.h> 
  
using namespace std;

int main(int argc, char** argv) {

#ifdef __WIN32__
    WORD versionWanted = MAKEWORD(1, 1);
    WSADATA wsaData;
    WSAStartup(versionWanted, &wsaData);
#endif
    // creating socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(65000);
    serverAddress.sin_addr.s_addr = inet_addr(argv[2]);

    // recieving data
    char buff[1024] = { 0 };

    if (strcmp(argv[1], "-c") == 0) {
        if (connect(serverSocket, (struct sockaddr*)&serverAddress,
                sizeof(serverAddress)) != 0) throw "oopsie";

        string msg;
        getline(cin, msg);
        send(serverSocket, msg.c_str(), msg.length(), 0);
        cout << "sent" << endl << endl;

        recv(serverSocket, buff, sizeof(buff), 0);
        cout << buff << endl;

    } else {
        if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) cout << "ERR" << endl;
        listen(serverSocket, 1);
        SOCKET clientSocket  = accept(serverSocket, nullptr, nullptr);

        recv(clientSocket , buff, sizeof(buff), 0);
        cout << "Message from client: " << buff << endl;

        if (strcmp(buff, "ping") == 0) send(clientSocket, "pong", 4, 0);
        else send(clientSocket, "not pong", 8, 0);
    }
    // closing the socket.
    close(serverSocket);

    return 0;
}