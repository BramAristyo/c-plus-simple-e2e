#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cmath>
#include "helper/crypt.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int g = 5;
const int p = 23;


int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9090);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    result = connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (result == SOCKET_ERROR) {
        cerr << "Connection failed: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    int b = 15;
    long long B = mod_exp(g, b, p);

    long long A;
    recv(clientSocket, (char*)&A, sizeof(A), 0);

    send(clientSocket, (char*)&B, sizeof(B), 0);

    long long sharedKey = mod_exp(A, b, p);
    cout << "Shared Key (Client): " << sharedKey << endl;

    while (true) {
        string message;
        cout << "Enter message to send to server: ";
        getline(cin, message);

        xor_encrypt_decrypt(message, sharedKey);
        send(clientSocket, message.c_str(), message.size() + 1, 0);

        char buffer[1024] = {0};
        result = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (result > 0) {
            string response(buffer);
            xor_encrypt_decrypt(response, sharedKey);
            cout << "Decrypted response from server: " << response << endl;
        } else if (result == 0) {
            cout << "Connection closed by server." << endl;
            break;
        } else {
            cerr << "Receive failed: " << WSAGetLastError() << endl;
            break;
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
