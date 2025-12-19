#ifndef ERROR_DETECTION_H
#define ERROR_DETECTION_H

#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdint>

// Error detection method types
enum class ErrorDetectionMethod {
    PARITY,
    PARITY_2D,
    CRC16,
    HAMMING,
    CHECKSUM
};

// Utility functions
class ErrorDetection {
public:
    // Convert string to binary representation
    static std::string stringToBinary(const std::string& str) {
        std::string binary;
        for (char c : str) {
            binary += std::bitset<8>(c).to_string();
        }
        return binary;
    }

    // Count number of 1s in binary string
    static int countOnes(const std::string& binary) {
        return std::count(binary.begin(), binary.end(), '1');
    }

    // 1. Parity Bit (Even/Odd)
    static std::string calculateParity(const std::string& data, bool evenParity = true) {
        std::string binary = stringToBinary(data);
        int ones = countOnes(binary);
        bool parityBit = evenParity ? (ones % 2 != 0) : (ones % 2 == 0);
        return parityBit ? "1" : "0";
    }

    // 2. 2D Parity (Matrix Parity)
    static std::string calculate2DParity(const std::string& data, int rows = 8) {
        if (data.empty()) return "0|0";
        
        // Convert to binary first
        std::string binary = stringToBinary(data);
        
        int cols = (binary.length() + rows - 1) / rows; // Ceiling division
        std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols, false));
        
        // Fill matrix with data bits
        for (size_t i = 0; i < binary.length(); i++) {
            int row = i % rows;
            int col = i / rows;
            if (col < cols) {
                matrix[row][col] = (binary[i] == '1');
            }
        }
        
        // Calculate row parities
        std::string rowParity;
        for (int i = 0; i < rows; i++) {
            int ones = 0;
            for (int j = 0; j < cols; j++) {
                if (matrix[i][j]) ones++;
            }
            rowParity += (ones % 2 == 0) ? "0" : "1";
        }
        
        // Calculate column parities
        std::string colParity;
        for (int j = 0; j < cols; j++) {
            int ones = 0;
            for (int i = 0; i < rows; i++) {
                if (matrix[i][j]) ones++;
            }
            colParity += (ones % 2 == 0) ? "0" : "1";
        }
        
        // Convert binary strings to hex for compact representation
        auto binaryToHex = [](const std::string& binary) -> std::string {
            std::stringstream ss;
            ss << std::hex << std::uppercase << std::setfill('0');
            
            // Pad to multiple of 4 bits for hex conversion
            std::string padded = binary;
            while (padded.length() % 4 != 0) {
                padded = "0" + padded;
            }
            
            // Convert each 4-bit group to hex
            for (size_t i = 0; i < padded.length(); i += 4) {
                std::string nibble = padded.substr(i, 4);
                uint8_t val = static_cast<uint8_t>(std::bitset<4>(nibble).to_ulong());
                ss << std::setw(1) << val;
            }
            
            return ss.str();
        };
        
        std::string rowHex = binaryToHex(rowParity);
        std::string colHex = binaryToHex(colParity);
        
        return rowHex + "|" + colHex;
    }

    // 3. CRC-16 (using polynomial 0x8005)
    static std::string calculateCRC16(const std::string& data) {
        uint16_t crc = 0xFFFF;
        uint16_t polynomial = 0x8005;
        
        for (char c : data) {
            crc ^= (static_cast<uint16_t>(c) << 8);
            for (int i = 0; i < 8; i++) {
                if (crc & 0x8000) {
                    crc = (crc << 1) ^ polynomial;
                } else {
                    crc <<= 1;
                }
            }
        }
        
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << crc;
        return ss.str();
    }

    // 4. Hamming Code (for 4-bit blocks)
    static std::string calculateHamming(const std::string& data) {
        std::string binary = stringToBinary(data);
        std::string hammingResult;
        
        // Process in 4-bit blocks
        for (size_t i = 0; i < binary.length(); i += 4) {
            std::string block = binary.substr(i, 4);
            if (block.length() < 4) {
                block += std::string(4 - block.length(), '0');
            }
            
            // Calculate parity bits for Hamming(7,4)
            // Positions: 1, 2, 4 are parity bits
            bool p1 = (block[0] - '0') ^ (block[1] - '0') ^ (block[3] - '0');
            bool p2 = (block[0] - '0') ^ (block[2] - '0') ^ (block[3] - '0');
            bool p4 = (block[1] - '0') ^ (block[2] - '0') ^ (block[3] - '0');
            
            hammingResult += std::to_string(p1) + std::to_string(p2) + block[0] + 
                           std::to_string(p4) + block.substr(1);
        }
        
        // Return a checksum of the hamming codes
        int checksum = 0;
        for (char c : hammingResult) {
            checksum += (c - '0');
        }
        
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << checksum;
        return ss.str();
    }

    // 5. Internet Checksum (16-bit)
    static std::string calculateChecksum(const std::string& data) {
        uint32_t sum = 0;
        
        // Process in 16-bit chunks
        for (size_t i = 0; i < data.length(); i += 2) {
            uint16_t word = 0;
            if (i < data.length()) {
                word = static_cast<uint8_t>(data[i]) << 8;
            }
            if (i + 1 < data.length()) {
                word |= static_cast<uint8_t>(data[i + 1]);
            }
            sum += word;
        }
        
        // Add carry bits
        while (sum >> 16) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
        
        // One's complement
        uint16_t checksum = ~static_cast<uint16_t>(sum);
        
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << checksum;
        return ss.str();
    }

    // Generate control information based on method
    static std::string generateControlInfo(const std::string& data, ErrorDetectionMethod method) {
        switch (method) {
            case ErrorDetectionMethod::PARITY:
                return calculateParity(data, true);
            case ErrorDetectionMethod::PARITY_2D:
                return calculate2DParity(data);
            case ErrorDetectionMethod::CRC16:
                return calculateCRC16(data);
            case ErrorDetectionMethod::HAMMING:
                return calculateHamming(data);
            case ErrorDetectionMethod::CHECKSUM:
                return calculateChecksum(data);
            default:
                return "";
        }
    }

    // Get method name as string
    static std::string methodToString(ErrorDetectionMethod method) {
        switch (method) {
            case ErrorDetectionMethod::PARITY: return "PARITY";
            case ErrorDetectionMethod::PARITY_2D: return "PARITY2D";
            case ErrorDetectionMethod::CRC16: return "CRC16";
            case ErrorDetectionMethod::HAMMING: return "HAMMING";
            case ErrorDetectionMethod::CHECKSUM: return "CHECKSUM";
            default: return "UNKNOWN";
        }
    }

    // Parse method from string
    static ErrorDetectionMethod stringToMethod(const std::string& methodStr) {
        if (methodStr == "PARITY") return ErrorDetectionMethod::PARITY;
        if (methodStr == "PARITY2D") return ErrorDetectionMethod::PARITY_2D;
        if (methodStr == "CRC16") return ErrorDetectionMethod::CRC16;
        if (methodStr == "HAMMING") return ErrorDetectionMethod::HAMMING;
        if (methodStr == "CHECKSUM") return ErrorDetectionMethod::CHECKSUM;
        return ErrorDetectionMethod::PARITY;
    }
};

#endif // ERROR_DETECTION_H

