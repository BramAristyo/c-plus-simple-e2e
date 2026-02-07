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

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed: " << WSAGetLastError() << endl;  
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9090);  
    serverAddress.sin_addr.s_addr = INADDR_ANY;  

    result = bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (result == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError() << endl;  
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    result = listen(serverSocket, 5);
    if (result == SOCKET_ERROR) {
        cerr << "Listen failed: " << WSAGetLastError() << endl;  
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port 9090..." << endl;

    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Accept failed: " << WSAGetLastError() << endl;  
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Diffie-Hellman key exchange: Server's private key
    int a = 6;  
    long long A = mod_exp(g, a, p);  

    send(clientSocket, (char*)&A, sizeof(A), 0);

    long long B;
    recv(clientSocket, (char*)&B, sizeof(B), 0);

    long long sharedKey = mod_exp(B, a, p);
    cout << "Shared Key (Server): " << sharedKey << endl;  

    // Loop to receive and respond to encrypted messages
    while (true) {
        char buffer[1024] = {0};  
        result = recv(clientSocket, buffer, sizeof(buffer), 0);  
        if (result > 0) {
            string encryptedMessage(buffer);  
            cout << "Encrypted message from Client: " << encryptedMessage << endl;

            xor_encrypt_decrypt(encryptedMessage, sharedKey);
            cout << "Client: " << encryptedMessage << endl;
            cout << "===========================================================" << endl;

            string response = "Message received: " + encryptedMessage;
            xor_encrypt_decrypt(response, sharedKey);  
            send(clientSocket, response.c_str(), response.size() + 1, 0);  
        } else if (result == 0) {
            cout << "Connection closed by client." << endl;
            break;
        } else {
            cerr << "Receive failed: " << WSAGetLastError() << endl;  
            break;
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
