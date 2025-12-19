#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "error_detection.h"
#include "error_injection.h"

#define SERVER_PORT 8080
#define CLIENT2_PORT 8081

int main() {
    // Create listening socket for Client 1
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    // Set socket option to reuse address
    int opt = 1;
    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Setup server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(listenSocket);
        return 1;
    }

    // Listen for connections
    if (listen(listenSocket, 5) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(listenSocket);
        return 1;
    }

    std::cout << "=== Server: Intermediate Node + Data Corruptor ===" << std::endl;
    std::cout << "Waiting for Client 1 on port " << SERVER_PORT << "..." << std::endl;

    // Accept connection from Client 1
    sockaddr_in client1Addr;
    socklen_t client1AddrLen = sizeof(client1Addr);
    int client1Socket = accept(listenSocket, (sockaddr*)&client1Addr, &client1AddrLen);
    if (client1Socket < 0) {
        std::cerr << "Accept failed" << std::endl;
        close(listenSocket);
        return 1;
    }

    std::cout << "Client 1 connected!" << std::endl;

    // Receive packet from Client 1
    char buffer[4096];
    ssize_t bytesReceived = recv(client1Socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0 || bytesReceived == 0) {
        std::cerr << "Receive failed" << std::endl;
        close(client1Socket);
        close(listenSocket);
        return 1;
    }

    buffer[bytesReceived] = '\0';
    std::string packet(buffer);

    std::cout << "\nReceived packet from Client 1: " << packet << std::endl;

    // Parse packet: DATA|METHOD|CONTROL_INFORMATION
    size_t firstPipe = packet.find('|');
    size_t secondPipe = packet.find('|', firstPipe + 1);

    if (firstPipe == std::string::npos || secondPipe == std::string::npos) {
        std::cerr << "Invalid packet format" << std::endl;
        close(client1Socket);
        close(listenSocket);
        return 1;
    }

    std::string data = packet.substr(0, firstPipe);
    std::string method = packet.substr(firstPipe + 1, secondPipe - firstPipe - 1);
    std::string controlInfo = packet.substr(secondPipe + 1);

    std::cout << "\nParsed Packet:" << std::endl;
    std::cout << "Data: " << data << std::endl;
    std::cout << "Method: " << method << std::endl;
    std::cout << "Control Info: " << controlInfo << std::endl;

    // Inject error
    std::string corruptedData = ErrorInjection::injectError(data);
    
    std::cout << "\nError Injection Applied:" << std::endl;
    std::cout << "Original Data: " << data << std::endl;
    std::cout << "Corrupted Data: " << corruptedData << std::endl;

    // Create new packet with corrupted data (keep same method and control info)
    std::string corruptedPacket = corruptedData + "|" + method + "|" + controlInfo;

    // Close connection with Client 1
    close(client1Socket);

    // Connect to Client 2
    int client2Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client2Socket < 0) {
        std::cerr << "Socket creation for Client 2 failed" << std::endl;
        close(listenSocket);
        return 1;
    }

    sockaddr_in client2Addr;
    client2Addr.sin_family = AF_INET;
    client2Addr.sin_port = htons(CLIENT2_PORT);
    if (inet_aton("127.0.0.1", &client2Addr.sin_addr) == 0) {
        std::cerr << "Invalid address" << std::endl;
        close(client2Socket);
        close(listenSocket);
        return 1;
    }

    std::cout << "\nConnecting to Client 2 on port " << CLIENT2_PORT << "..." << std::endl;

    if (connect(client2Socket, (sockaddr*)&client2Addr, sizeof(client2Addr)) < 0) {
        std::cerr << "Connection to Client 2 failed. Make sure Client 2 is running." << std::endl;
        close(client2Socket);
        close(listenSocket);
        return 1;
    }

    std::cout << "Connected to Client 2!" << std::endl;

    // Send corrupted packet to Client 2
    ssize_t bytesSent = send(client2Socket, corruptedPacket.c_str(), corruptedPacket.length(), 0);
    if (bytesSent < 0) {
        std::cerr << "Send to Client 2 failed" << std::endl;
    } else {
        std::cout << "Corrupted packet sent to Client 2 (" << bytesSent << " bytes)" << std::endl;
    }

    // Close sockets
    close(client2Socket);
    close(listenSocket);

    std::cout << "\nServer finished." << std::endl;
    return 0;
}

