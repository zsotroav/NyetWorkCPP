// Loosely based on https://www.geeksforgeeks.org/socket-programming-in-cpp/

#include <cstring> 
#include <iostream>
#include <sstream>
#include <unistd.h>

#ifdef __WIN32__
# include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
  
using namespace std;

int main(int argc, char** argv) {

    if (argc != 3) {
        cout << "Usage: <mode> <ip>\n"
                "\n"
                "Mode: \n"
                "      -c client (Sender)\n"
                "      -s server (Receiver)" << endl;
        return -1;
    }

#ifdef __WIN32__
    // Init winsock
    WORD versionWanted = MAKEWORD(1, 1);
    WSADATA wsaData;
    WSAStartup(versionWanted, &wsaData);
#endif
    // creating server socket
    auto serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(65000); // TCP PORT
    serverAddress.sin_addr.s_addr = inet_addr(argv[2]);

    char buffer[1024] = { 0 };

    if (strcmp(argv[1], "-c") == 0) {
        // Try to connect to server
        int res = connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

        // Write status code
        stringstream ss;
        ss << "Connection code: " << res;
        cout << ss.str() << endl;
        if (res != 0) throw std::runtime_error(ss.str());

        // Get message to send
        cout << "Message to send:" << endl;
        string msg;
        getline(cin, msg);

        // Send message to the server
        send(serverSocket, msg.c_str(), msg.length(), 0);
        cout << "sent message" << endl << endl;

        // Get response from the server
        recv(serverSocket, buffer, sizeof(buffer), 0);
        cout << "Response: " << buffer << endl;

    } else if (strcmp(argv[1], "-s") == 0) {
        // Try to bind port (to own ip)
        int res = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

        // Write status code
        stringstream ss;
        ss << "Socket bind: " << res;
        cout << ss.str() << endl;
        if (res != 0) throw std::runtime_error(ss.str());

        // Wait for connection
        listen(serverSocket, 1);
        auto clientSocket  = accept(serverSocket, nullptr, nullptr);

        // Get message
        recv(clientSocket , buffer, sizeof(buffer), 0);
        cout << "Message from client: " << buffer << endl;

        // Write back a response
        if (strcmp(buffer, "ping") == 0) send(clientSocket, "pong", 4, 0);
        else send(clientSocket, "not pong", 8, 0);
    } else {
        cout << "Usage: <mode> <ip>\n"
                "\n"
                "Mode: \n"
                "      -c client (Sender)\n"
                "      -s server (Receiver)" << endl;
        return -1;
    }
    // closing the socket.
    close(serverSocket);

#ifdef __WIN32__
    WSACleanup();
#endif

    return 0;
}