#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void ShowErrorMessage(string message) {
    cout << "[Error detected]: " << message << endl;
    exit(1);
}

int main() {
    
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN serverAddress, clientAddress;

    int serverPort = 9876;
    char received[256];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ShowErrorMessage("WSAStartup()");
    
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
        ShowErrorMessage("socket()");
    
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(serverPort);
    if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
        ShowErrorMessage("bind()");
    cout << "[Current state]: bind()" << endl;
    
    if (listen(serverSocket, 5) == SOCKET_ERROR)
        ShowErrorMessage("listen()");
    cout << "[Current state]: listen()" << endl;

    int sizeClientAddress = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
    if (clientSocket == INVALID_SOCKET)
        ShowErrorMessage("accept()");
    cout << "[Current state]: accept()" << endl;

    while(1) {
        int Msg = recv(clientSocket, received, sizeof(received), 0);
        received[Msg] = NULL;
        cout << "[Client Message]: " << received << endl;
        cout << "[Send Message]: " << received << endl;
        if (strcmp(received, "[exit]") == 0) {
            send(clientSocket, received, sizeof(received) - 1, 0);
            cout << "[Server exit]" << endl;
            break;
        }
        send(clientSocket, received, sizeof(received) - 1, 0);
    }

    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}