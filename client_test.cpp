#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>


using namespace std;

void ShowErrorMessage(string message) {
    cout << "[Error detected]: " << message << endl;
    exit(1);
}

int main() {
    
    WSADATA wsaData;
    SOCKET clientSocket;
    SOCKADDR_IN serverAddress;

    int serverPort = 9876;
    char received[256];
    string sent;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ShowErrorMessage("WSAStartup()");
    
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
        ShowErrorMessage("socket()");
    
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("192.168.0.62");
    serverAddress.sin_port = htons(serverPort);
    if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
        ShowErrorMessage("connect()");
    cout << "[Current state]: connect()" << endl;

    while(1) {
        cout << "[Message send]: ";
        getline(cin, sent);
        if (sent == "") continue;
        send(clientSocket, sent.c_str(), sent.length(), 0);
        int Msg = recv(clientSocket, received, sizeof(received), 0);
        received[Msg] = NULL;
        if (strcmp(received, "[exit]") == 0) {
            cout << "[Server exit]" << endl;
            break;
        }
        cout << "[Server message]: " << received << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}