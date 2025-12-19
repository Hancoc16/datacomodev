#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "error_detection.h"

#define CLIENT2_PORT 8081

int main() {
    // Create listening socket
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
    serverAddr.sin_port = htons(CLIENT2_PORT);
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

    std::cout << "=== Client 2: Receiver + Error Checker ===" << std::endl;
    std::cout << "Waiting for server on port " << CLIENT2_PORT << "..." << std::endl;

    // Accept connection from server
    sockaddr_in serverAddr2;
    socklen_t serverAddrLen = sizeof(serverAddr2);
    int serverSocket = accept(listenSocket, (sockaddr*)&serverAddr2, &serverAddrLen);
    if (serverSocket < 0) {
        std::cerr << "Accept failed" << std::endl;
        close(listenSocket);
        return 1;
    }

    std::cout << "Server connected!" << std::endl;

    // Receive packet from server
    char buffer[4096];
    ssize_t bytesReceived = recv(serverSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0 || bytesReceived == 0) {
        std::cerr << "Receive failed" << std::endl;
        close(serverSocket);
        close(listenSocket);
        return 1;
    }

    buffer[bytesReceived] = '\0';
    std::string packet(buffer);

    std::cout << "\nReceived packet from server: " << packet << std::endl;

    // Parse packet: DATA|METHOD|CONTROL_INFORMATION
    size_t firstPipe = packet.find('|');
    size_t secondPipe = packet.find('|', firstPipe + 1);

    if (firstPipe == std::string::npos || secondPipe == std::string::npos) {
        std::cerr << "Invalid packet format" << std::endl;
        close(serverSocket);
        close(listenSocket);
        return 1;
    }

    std::string receivedData = packet.substr(0, firstPipe);
    std::string methodStr = packet.substr(firstPipe + 1, secondPipe - firstPipe - 1);
    std::string incomingControl = packet.substr(secondPipe + 1);

    std::cout << "\n=== Error Detection Results ===" << std::endl;
    std::cout << "Received Data : " << receivedData << std::endl;
    std::cout << "Method : " << methodStr << std::endl;
    std::cout << "Sent Check Bits : " << incomingControl << std::endl;

    // Recalculate control information based on method
    ErrorDetectionMethod method = ErrorDetection::stringToMethod(methodStr);
    std::string computedControl;

    switch (method) {
        case ErrorDetectionMethod::PARITY:
            computedControl = ErrorDetection::calculateParity(receivedData, true);
            break;
        case ErrorDetectionMethod::PARITY_2D:
            computedControl = ErrorDetection::calculate2DParity(receivedData);
            break;
        case ErrorDetectionMethod::CRC16:
            computedControl = ErrorDetection::calculateCRC16(receivedData);
            break;
        case ErrorDetectionMethod::HAMMING:
            computedControl = ErrorDetection::calculateHamming(receivedData);
            break;
        case ErrorDetectionMethod::CHECKSUM:
            computedControl = ErrorDetection::calculateChecksum(receivedData);
            break;
        default:
            std::cerr << "Unknown method" << std::endl;
            close(serverSocket);
            close(listenSocket);
            return 1;
    }

    std::cout << "Computed Check Bits : " << computedControl << std::endl;

    // Compare control information
    bool isCorrect = (incomingControl == computedControl);

    std::cout << "Status: " << (isCorrect ? "DATA CORRECT" : "DATA CORRUPTED") << std::endl;

    // Close sockets
    close(serverSocket);
    close(listenSocket);

    return 0;
}

