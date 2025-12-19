#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "error_detection.h"

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    // Create socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    // Setup server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_aton(SERVER_IP, &serverAddr.sin_addr) == 0) {
        std::cerr << "Invalid address" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Connect to server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(clientSocket);
        return 1;
    }

    std::cout << "Connected to server!" << std::endl;
    std::cout << "\n=== Client 1: Data Sender ===" << std::endl;

    // Get input from user
    std::string data;
    std::cout << "Enter data to send: ";
    std::getline(std::cin, data);

    if (data.empty()) {
        std::cout << "Empty data, exiting..." << std::endl;
        close(clientSocket);
        return 0;
    }

    // Select error detection method
    std::cout << "\nSelect error detection method:" << std::endl;
    std::cout << "1. Parity Bit" << std::endl;
    std::cout << "2. 2D Parity" << std::endl;
    std::cout << "3. CRC-16" << std::endl;
    std::cout << "4. Hamming Code" << std::endl;
    std::cout << "5. Internet Checksum" << std::endl;
    std::cout << "Choice (1-5): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Clear newline

    std::string methodStr;
    std::string controlInfo;

    switch (choice) {
        case 1:
            methodStr = "PARITY";
            controlInfo = ErrorDetection::calculateParity(data, true);
            break;
        case 2:
            methodStr = "PARITY2D";
            controlInfo = ErrorDetection::calculate2DParity(data);
            break;
        case 3:
            methodStr = "CRC16";
            controlInfo = ErrorDetection::calculateCRC16(data);
            break;
        case 4:
            methodStr = "HAMMING";
            controlInfo = ErrorDetection::calculateHamming(data);
            break;
        case 5:
            methodStr = "CHECKSUM";
            controlInfo = ErrorDetection::calculateChecksum(data);
            break;
        default:
            std::cout << "Invalid choice, using Parity Bit" << std::endl;
            methodStr = "PARITY";
            controlInfo = ErrorDetection::calculateParity(data, true);
            break;
    }

    // Create packet: DATA|METHOD|CONTROL_INFORMATION
    std::string packet = data + "|" + methodStr + "|" + controlInfo;

    std::cout << "\nGenerated Packet:" << std::endl;
    std::cout << "Data: " << data << std::endl;
    std::cout << "Method: " << methodStr << std::endl;
    std::cout << "Control Information: " << controlInfo << std::endl;
    std::cout << "Full Packet: " << packet << std::endl;

    // Send packet to server
    ssize_t bytesSent = send(clientSocket, packet.c_str(), packet.length(), 0);
    if (bytesSent < 0) {
        std::cerr << "Send failed" << std::endl;
    } else {
        std::cout << "\nPacket sent successfully (" << bytesSent << " bytes)" << std::endl;
    }

    // Close socket
    close(clientSocket);

    return 0;
}

